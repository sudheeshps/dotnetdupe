#include "pch.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/WaitHandleCannotBeOpenedException.h"
#include "System/Utils/StringConvert.h"
#include "System/SmartPointer.h"
#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            AutoResetEvent::AutoResetEvent(bool initialState)
                : EventWaitHandle(initialState, false) {
                /// Step: Initialize unnamed auto-reset event with initial signaling state.
            }

            static bool s_autoDummyCreatedNew = false;
            AutoResetEvent::AutoResetEvent(const String& sName, bool initialState, bool openAlways)
                : EventWaitHandle(initialState, false, sName, openAlways, s_autoDummyCreatedNew) {
                /// Step: Initialize named auto-reset event.
            }

            AutoResetEvent::AutoResetEvent(bool initialState, const String& sName, bool openAlways)
                : EventWaitHandle(initialState, false, sName, openAlways, s_autoDummyCreatedNew) {
                /// Step: Initialize named auto-reset event overload.
            }

            AutoResetEvent::AutoResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew)
                : EventWaitHandle(initialState, false, sName, openAlways, bCreatedNew) {
                /// Step: Initialize named auto-reset event tracking createdNew status.
            }

            SmartPointer<AutoResetEvent> AutoResetEvent::OpenExisting(const String& sName) {
                /// Open existing system auto-reset event.
                SmartPointer<AutoResetEvent> pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No AutoResetEvent handle of the given name exists.");
            }

            bool AutoResetEvent::TryOpenExisting(const String& sName, SmartPointer<AutoResetEvent>& pResult) {
                /// Guard: Check empty name.
                pResult = nullptr;
                if (sName.IsEmpty()) return false;

#if defined(_WIN32)
                /// Attempt Win32 OpenEventW.
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (!h) return false;

                auto spEvt = SmartPointer<AutoResetEvent>::NewShared(false);
                spEvt->_name = sName;
                spEvt->_hHandle = h;
                pResult = std::move(spEvt);
                return true;
#else
                return false;
#endif
            }
        }
    }
}