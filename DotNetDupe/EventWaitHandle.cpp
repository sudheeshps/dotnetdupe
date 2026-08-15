#include "pch.h"
#include "System/Threading/EventWaitHandle.h"
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

            struct EventWaitHandle::Impl {
                std::mutex mutex;
                std::condition_variable cv;
            };

            EventWaitHandle::EventWaitHandle(bool initialState, bool manualReset)
                : _state(initialState), _manualReset(manualReset), _name(""), _hHandle(nullptr), _pImpl(new Impl()) {}

            static bool s_dummyCreatedNew = false;
            EventWaitHandle::EventWaitHandle(const String& sName, bool initialState, bool manualReset, bool openAlways)
                : EventWaitHandle(initialState, manualReset, sName, openAlways, s_dummyCreatedNew) {}

            EventWaitHandle::EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways)
                : EventWaitHandle(initialState, manualReset, sName, openAlways, s_dummyCreatedNew) {}

#if defined(_WIN32)
            static HANDLE OpenOrCreateWin32Event(const std::wstring& wsName, bool manualReset, bool initialState, bool openAlways, bool& bCreatedNew) {
                HANDLE hHandle = ::CreateEventW(NULL, manualReset ? TRUE : FALSE, initialState ? TRUE : FALSE, wsName.c_str());
                if (hHandle != NULL) {
                    bCreatedNew = (::GetLastError() != ERROR_ALREADY_EXISTS);
                    return hHandle;
                }
                if (::GetLastError() == ERROR_ACCESS_DENIED) {
                    throw UnauthorizedAccessException("Access denied creating EventWaitHandle synchronization object.");
                }
                bCreatedNew = false;
                if (!openAlways) {
                    throw WaitHandleCannotBeOpenedException("Event creation returned null handle and openAlways is false.");
                }
                hHandle = ::OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (hHandle == NULL) {
                    if (::GetLastError() == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied opening existing EventWaitHandle synchronization object.");
                    }
                    throw WaitHandleCannotBeOpenedException("Failed to open existing event with SYNCHRONIZE access.");
                }
                return hHandle;
            }
#endif

            EventWaitHandle::EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways, bool& bCreatedNew)
                : _state(initialState), _manualReset(manualReset), _name(sName), _hHandle(nullptr), _pImpl(new Impl()) {
#if defined(_WIN32)
                if (!_name.IsEmpty()) {
                    std::wstring wsName = Utils::StringConvert::Utf8ToWChar(_name.GetRawString());
                    _hHandle = OpenOrCreateWin32Event(wsName, manualReset, initialState, openAlways, bCreatedNew);
                } else {
                    bCreatedNew = true;
                }
#else
                bCreatedNew = true;
#endif
            }

            EventWaitHandle::~EventWaitHandle() {
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

            SmartPointer<EventWaitHandle> EventWaitHandle::OpenExisting(const String& sName) {
                SmartPointer<EventWaitHandle> pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No event handle of the given name exists.");
            }

            bool EventWaitHandle::TryOpenExisting(const String& sName, SmartPointer<EventWaitHandle>& pResult) {
                pResult = nullptr;
                if (sName.IsEmpty()) return false;
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (!h) return false;
                auto spEvt = SmartPointer<EventWaitHandle>::NewShared(false, false);
                spEvt->_name = sName;
                spEvt->_hHandle = h;
                pResult = std::move(spEvt);
                return true;
#else
                return false;
#endif
            }

            bool EventWaitHandle::Set() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    return (::SetEvent((HANDLE)_hHandle) != FALSE);
                }
#endif
                if (!_pImpl) return false;
                std::lock_guard<std::mutex> lock(_pImpl->mutex);
                _state = true;
                if (_manualReset) {
                    _pImpl->cv.notify_all();
                } else {
                    _pImpl->cv.notify_one();
                }
                return true;
            }

            bool EventWaitHandle::Reset() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    return (::ResetEvent((HANDLE)_hHandle) != FALSE);
                }
#endif
                if (!_pImpl) return false;
                std::lock_guard<std::mutex> lock(_pImpl->mutex);
                _state = false;
                return true;
            }

            static bool WaitForEventCv(EventWaitHandle::Impl* pImpl, bool& bState, bool bManualReset) {
                std::unique_lock<std::mutex> lock(pImpl->mutex);
                pImpl->cv.wait(lock, [&bState]() { return bState; });
                if (!bManualReset) bState = false;
                return true;
            }

            static bool WaitForEventCv(EventWaitHandle::Impl* pImpl, bool& bState, bool bManualReset, int msTimeout) {
                std::unique_lock<std::mutex> lock(pImpl->mutex);
                bool bRes = pImpl->cv.wait_for(lock, std::chrono::milliseconds(msTimeout), [&bState]() { return bState; });
                if (bRes) {
                    if (!bManualReset) bState = false;
                    return true;
                }
                throw TimeoutException("The wait operation timed out.");
            }

            bool EventWaitHandle::WaitOne() {
#if defined(_WIN32)
                if (_hHandle != nullptr) {
                    DWORD dwWaitResult = ::WaitForSingleObject((HANDLE)_hHandle, INFINITE);
                    return (dwWaitResult == WAIT_OBJECT_0);
                }
#endif
                if (!_pImpl) return false;
                return WaitForEventCv(_pImpl, _state, _manualReset);
            }

            bool EventWaitHandle::WaitOne(int millisecondsTimeout) {
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
                return WaitForEventCv(_pImpl, _state, _manualReset, millisecondsTimeout);
            }
        }
    }
}