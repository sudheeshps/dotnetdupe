#include "pch.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/WaitHandleCannotBeOpenedException.h"
#include "System/TimeoutException.h"
#include "System/UnauthorizedAccessException.h"
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
            Mutex::Mutex() : _name(""), _hHandle(nullptr) {}

            Mutex::Mutex(bool bInitiallyOwned) : _name(""), _hHandle(nullptr) {
                if (bInitiallyOwned) {
                    _mutex.lock();
                }
            }

            static bool s_mutexDummyCreatedNew = false;
            Mutex::Mutex(const String& sName, bool bInitiallyOwned, bool openAlways)
                : Mutex(bInitiallyOwned, sName, openAlways, s_mutexDummyCreatedNew) {}

            Mutex::Mutex(bool bInitiallyOwned, const String& sName, bool openAlways)
                : Mutex(bInitiallyOwned, sName, openAlways, s_mutexDummyCreatedNew) {}

#if defined(_WIN32)
            static HANDLE OpenOrCreateWin32Mutex(const std::wstring& wsName, bool bInitiallyOwned, bool openAlways, bool& bCreatedNew) {
                HANDLE hHandle = ::CreateMutexW(NULL, bInitiallyOwned ? TRUE : FALSE, wsName.c_str());
                if (hHandle != NULL) {
                    bCreatedNew = (::GetLastError() != ERROR_ALREADY_EXISTS);
                    return hHandle;
                }
                if (::GetLastError() == ERROR_ACCESS_DENIED) {
                    throw UnauthorizedAccessException("Access denied creating Mutex synchronization object.");
                }
                bCreatedNew = false;
                if (!openAlways) {
                    throw WaitHandleCannotBeOpenedException("Mutex creation returned null handle and openAlways is false.");
                }
                hHandle = ::OpenMutexW(SYNCHRONIZE, FALSE, wsName.c_str());
                if (hHandle == NULL) {
                    if (::GetLastError() == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied opening existing Mutex synchronization object.");
                    }
                    throw WaitHandleCannotBeOpenedException("Failed to open existing mutex with SYNCHRONIZE access.");
                }
                return hHandle;
            }
#endif

            Mutex::Mutex(bool bInitiallyOwned, const String& sName, bool openAlways, bool& bCreatedNew)
                : _name(sName), _hHandle(nullptr) {
#if defined(_WIN32)
                if (!_name.IsEmpty()) {
                    std::wstring wsName = Utils::StringConvert::Utf8ToWChar(_name.GetRawString());
                    _hHandle = OpenOrCreateWin32Mutex(wsName, bInitiallyOwned, openAlways, bCreatedNew);
                } else {
                    bCreatedNew = true;
                    if (bInitiallyOwned) {
                        _mutex.lock();
                    }
                }
#else
                bCreatedNew = true;
                if (bInitiallyOwned) {
                    _mutex.lock();
                }
#endif
            }

            Mutex::~Mutex() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    ::CloseHandle((HANDLE)_hHandle);
                    _hHandle = nullptr;
                }
#endif
            }

            SmartPointer<Mutex> Mutex::OpenExisting(const String& sName) {
                SmartPointer<Mutex> pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No mutex handle of the given name exists.");
            }

            bool Mutex::TryOpenExisting(const String& sName, SmartPointer<Mutex>& pResult) {
                pResult = SmartPointer<Mutex>();
                if (sName.IsEmpty()) return false;
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenMutexW(SYNCHRONIZE, FALSE, wsName.c_str());
                if (h == NULL) return false;
                SmartPointer<Mutex> spM = SmartPointer<Mutex>::NewShared();
                spM->_name = sName;
                spM->_hHandle = h;
                pResult = std::move(spM);
                return true;
#else
                return false;
#endif
            }

            bool Mutex::WaitOne() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    DWORD dwWaitResult = ::WaitForSingleObject((HANDLE)_hHandle, INFINITE);
                    return (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED);
                }
#endif
                _mutex.lock();
                return true;
            }

            bool Mutex::WaitOne(int millisecondsTimeout) {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    DWORD dwWaitResult = ::WaitForSingleObject((HANDLE)_hHandle, (DWORD)millisecondsTimeout);
                    if (dwWaitResult == WAIT_TIMEOUT) {
                        throw TimeoutException("The wait operation timed out.");
                    }
                    return (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED);
                }
#endif
                if (!_mutex.try_lock_for(std::chrono::milliseconds(millisecondsTimeout))) {
                    throw TimeoutException("The wait operation timed out.");
                }
                return true;
            }

            int Mutex::Release(int releaseCount) {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    ::ReleaseMutex((HANDLE)_hHandle);
                    return 0;
                }
#endif
                _mutex.unlock();
                return 0;
            }
        }
    }
}