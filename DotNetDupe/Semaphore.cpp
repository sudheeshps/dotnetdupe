#include "pch.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/Threading/WaitHandleCannotBeOpenedException.h"
#include "System/TimeoutException.h"
#include "System/Char.h"
#include "System/Utils/StringConvert.h"
#include "System/SmartPointer.h"
#include <chrono>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            Semaphore::Semaphore(int initialCount, int maximumCount)
                : _count(initialCount), _maxCount(maximumCount), _name(""), _hHandle(nullptr) {}

            static bool s_semDummyCreatedNew = false;
            Semaphore::Semaphore(const String& sName, int initialCount, int maximumCount, bool openAlways)
                : Semaphore(initialCount, maximumCount, sName, openAlways, s_semDummyCreatedNew) {}

            Semaphore::Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways)
                : Semaphore(initialCount, maximumCount, sName, openAlways, s_semDummyCreatedNew) {}

            Semaphore::Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways, bool& bCreatedNew)
                : _count(initialCount), _maxCount(maximumCount), _name(sName), _hHandle(nullptr) {
#if defined(_WIN32)
                if (!_name.IsEmpty()) {
                    std::wstring wsName = Utils::StringConvert::Utf8ToWChar(_name.GetRawString());
                    _hHandle = ::CreateSemaphoreW(NULL, initialCount, maximumCount, wsName.c_str());
                    if (_hHandle != NULL) {
                        bCreatedNew = (::GetLastError() != ERROR_ALREADY_EXISTS);
                    } else {
                        bCreatedNew = false;
                        if (openAlways) {
                            _hHandle = ::OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                            if (_hHandle == NULL) {
                                throw WaitHandleCannotBeOpenedException("Failed to open existing semaphore with SYNCHRONIZE access.");
                            }
                        } else {
                            throw WaitHandleCannotBeOpenedException("Semaphore creation returned null handle and openAlways is false.");
                        }
                    }
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
            }

            Semaphore* Semaphore::OpenExisting(const String& sName) {
                Semaphore* pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No semaphore handle of the given name exists.");
            }

            bool Semaphore::TryOpenExisting(const String& sName, Semaphore*& pResult) {
                pResult = nullptr;
                if (sName.IsEmpty()) {
                    return false;
                }
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (h != NULL) {
                    SmartPointer<Semaphore> spSem = SmartPointer<Semaphore>::New(0, 1);
                    spSem->_name = sName;
                    spSem->_hHandle = h;
                    pResult = spSem.Detach();
                    return true;
                }
                return false;
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
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return _count > 0; });
                --_count;
                return true;
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
                std::unique_lock<std::mutex> lock(_mutex);
                bool result = _cv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _count > 0; });
                if (result) {
                    --_count;
                } else {
                    throw TimeoutException("The wait operation timed out.");
                }
                return result;
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
                std::lock_guard<std::mutex> lock(_mutex);
                if (_count + releaseCount > _maxCount) {
                    throw SemaphoreFullException("Semaphore count exceeded maximum count.");
                }
                int prev = _count;
                _count += releaseCount;
                for (int i = 0; i < releaseCount; ++i) _cv.notify_one();
                return prev;
            }
        }
    }
}