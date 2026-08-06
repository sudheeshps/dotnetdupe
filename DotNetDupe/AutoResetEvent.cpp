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
                : EventWaitHandle(initialState, false) {}

            static bool s_autoDummyCreatedNew = false;
            AutoResetEvent::AutoResetEvent(const String& sName, bool initialState, bool openAlways)
                : EventWaitHandle(initialState, false, sName, openAlways, s_autoDummyCreatedNew) {}

            AutoResetEvent::AutoResetEvent(bool initialState, const String& sName, bool openAlways)
                : EventWaitHandle(initialState, false, sName, openAlways, s_autoDummyCreatedNew) {}

            AutoResetEvent::AutoResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew)
                : EventWaitHandle(initialState, false, sName, openAlways, bCreatedNew) {}

            AutoResetEvent* AutoResetEvent::OpenExisting(const String& sName) {
                AutoResetEvent* pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No AutoResetEvent handle of the given name exists.");
            }

            bool AutoResetEvent::TryOpenExisting(const String& sName, AutoResetEvent*& pResult) {
                pResult = nullptr;
                if (sName.IsEmpty()) {
                    return false;
                }
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (h != NULL) {
                    SmartPointer<AutoResetEvent> spEvt = SmartPointer<AutoResetEvent>::New(false);
                    spEvt->_name = sName;
                    spEvt->_hHandle = h;
                    pResult = spEvt.Detach();
                    return true;
                }
                return false;
#else
                return false;
#endif
            }
        }
    }
}