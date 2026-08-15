#include "pch.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/Threading/WaitHandleCannotBeOpenedException.h"
#include "System/TimeoutException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/Char.h"
#include "System/Utils/StringConvert.h"
#include "System/SmartPointer.h"
#include <chrono>
#include <mutex>
#include <condition_variable>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            struct Semaphore::Impl {
                std::mutex mutex;
                std::condition_variable cv;
            };

            Semaphore::Semaphore(int initialCount, int maximumCount)
                : _count(initialCount), _maxCount(maximumCount), _name(""), _hHandle(nullptr), _pImpl(new Impl()) {}

            static bool s_semDummyCreatedNew = false;
            Semaphore::Semaphore(const String& sName, int initialCount, int maximumCount, bool openAlways)
                : Semaphore(initialCount, maximumCount, sName, openAlways, s_semDummyCreatedNew) {}

            Semaphore::Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways)
                : Semaphore(initialCount, maximumCount, sName, openAlways, s_semDummyCreatedNew) {}

#if defined(_WIN32)
            static HANDLE OpenOrCreateWin32Semaphore(const std::wstring& wsName, int initialCount, int maximumCount, bool openAlways, bool& bCreatedNew) {
                HANDLE hHandle = ::CreateSemaphoreW(NULL, initialCount, maximumCount, wsName.c_str());
                if (hHandle != NULL) {
                    bCreatedNew = (::GetLastError() != ERROR_ALREADY_EXISTS);
                    return hHandle;
                }
                if (::GetLastError() == ERROR_ACCESS_DENIED) {
                    throw UnauthorizedAccessException("Access denied creating Semaphore synchronization object.");
                }
                bCreatedNew = false;
                if (!openAlways) {
                    throw WaitHandleCannotBeOpenedException("Semaphore creation returned null handle and openAlways is false.");
                }
                hHandle = ::OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (hHandle == NULL) {
                    if (::GetLastError() == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied opening existing Semaphore synchronization object.");
                    }
                    throw WaitHandleCannotBeOpenedException("Failed to open existing semaphore with SYNCHRONIZE access.");
                }
                return hHandle;
            }
#endif

            Semaphore::Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways, bool& bCreatedNew)
                : _count(initialCount), _maxCount(maximumCount), _name(sName), _hHandle(nullptr), _pImpl(new Impl()) {
#if defined(_WIN32)
                if (!_name.IsEmpty()) {
                    std::wstring wsName = Utils::StringConvert::Utf8ToWChar(_name.GetRawString());
                    _hHandle = OpenOrCreateWin32Semaphore(wsName, initialCount, maximumCount, openAlways, bCreatedNew);
                } else {
                    bCreatedNew = true;
                }
#else
                bCreatedNew = true;
#endif
            }

            Semaphore::~Semaphore() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    ::CloseHandle((HANDLE)_hHandle);
                    _hHandle = nullptr;
                }
#endif
                if (_pImpl != nullptr) {
                    delete _pImpl;
                    _pImpl = nullptr;
                }
            }

            SmartPointer<Semaphore> Semaphore::OpenExisting(const String& sName) {
                SmartPointer<Semaphore> pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No semaphore handle of the given name exists.");
            }

            bool Semaphore::TryOpenExisting(const String& sName, SmartPointer<Semaphore>& pResult) {
                pResult = SmartPointer<Semaphore>();
                if (sName.IsEmpty()) return false;
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (h == NULL) return false;
                SmartPointer<Semaphore> spSem = SmartPointer<Semaphore>::NewShared(0, 1);
                spSem->_name = sName;
                spSem->_hHandle = h;
                pResult = std::move(spSem);
                return true;
#else
                return false;
#endif
            }

            bool Semaphore::WaitOne() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    DWORD dwWaitResult = ::WaitForSingleObject((HANDLE)_hHandle, INFINITE);
                    return (dwWaitResult == WAIT_OBJECT_0);
                }
#endif
                if (!_pImpl) return false;
                std::unique_lock<std::mutex> lock(_pImpl->mutex);
                _pImpl->cv.wait(lock, [this]() { return _count > 0; });
                --_count;
                return true;
            }

            static bool WaitForSemaphoreCv(Semaphore::Impl* pImpl, int& count, int msTimeout) {
                std::unique_lock<std::mutex> lock(pImpl->mutex);
                bool bRes = pImpl->cv.wait_for(lock, std::chrono::milliseconds(msTimeout), [&count]() { return count > 0; });
                if (!bRes) throw TimeoutException("The wait operation timed out.");
                --count;
                return true;
            }

            static int ReleaseSemaphoreCv(Semaphore::Impl* pImpl, int& count, int maxCount, int releaseCount) {
                std::lock_guard<std::mutex> lock(pImpl->mutex);
                if (count + releaseCount > maxCount) {
                    throw SemaphoreFullException("Semaphore count exceeded maximum count.");
                }
                int prev = count;
                count += releaseCount;
                for (int i = 0; i < releaseCount; ++i) pImpl->cv.notify_one();
                return prev;
            }

            bool Semaphore::WaitOne(int millisecondsTimeout) {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    DWORD dwWaitResult = ::WaitForSingleObject((HANDLE)_hHandle, (DWORD)millisecondsTimeout);
                    if (dwWaitResult == WAIT_TIMEOUT) {
                        throw TimeoutException("The wait operation timed out.");
                    }
                    return (dwWaitResult == WAIT_OBJECT_0);
                }
#endif
                if (!_pImpl) return false;
                return WaitForSemaphoreCv(_pImpl, _count, millisecondsTimeout);
            }

            int Semaphore::Release(int releaseCount) {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    LONG previousCount = 0;
                    if (!::ReleaseSemaphore((HANDLE)_hHandle, releaseCount, &previousCount)) {
                        throw SemaphoreFullException("Semaphore count exceeded maximum count.");
                    }
                    return (int)previousCount;
                }
#endif
                if (!_pImpl) return 0;
                return ReleaseSemaphoreCv(_pImpl, _count, _maxCount, releaseCount);
            }
        }
    }
}