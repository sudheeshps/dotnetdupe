#include "pch.h"
#include "System/Threading/Mutex.h"
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

            Mutex::Mutex(bool bInitiallyOwned, const String& sName, bool openAlways, bool& bCreatedNew)
                : _name(sName), _hHandle(nullptr) {
#if defined(_WIN32)
                if (!_name.IsEmpty()) {
                    std::wstring wsName = Utils::StringConvert::Utf8ToWChar(_name.GetRawString());
                    _hHandle = ::CreateMutexW(NULL, bInitiallyOwned ? TRUE : FALSE, wsName.c_str());
                    if (_hHandle != NULL) {
                        bCreatedNew = (::GetLastError() != ERROR_ALREADY_EXISTS);
                    } else {
                        bCreatedNew = false;
                        if (openAlways) {
                            _hHandle = ::OpenMutexW(SYNCHRONIZE, FALSE, wsName.c_str());
                            if (_hHandle == NULL) {
                                throw WaitHandleCannotBeOpenedException("Failed to open existing mutex with SYNCHRONIZE access.");
                            }
                        } else {
                            throw WaitHandleCannotBeOpenedException("Mutex creation returned null handle and openAlways is false.");
                        }
                    }
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

            Mutex* Mutex::OpenExisting(const String& sName) {
                Mutex* pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No mutex handle of the given name exists.");
            }

            bool Mutex::TryOpenExisting(const String& sName, Mutex*& pResult) {
                pResult = nullptr;
                if (sName.IsEmpty()) {
                    return false;
                }
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenMutexW(SYNCHRONIZE, FALSE, wsName.c_str());
                if (h != NULL) {
                    SmartPointer<Mutex> spM = SmartPointer<Mutex>::New();
                    spM->_name = sName;
                    spM->_hHandle = h;
                    pResult = spM.Detach();
                    return true;
                }
                return false;
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