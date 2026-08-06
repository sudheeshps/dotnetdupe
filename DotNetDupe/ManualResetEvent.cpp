#include "pch.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/WaitHandleCannotBeOpenedException.h"
#include "System/Utils/StringConvert.h"
#include "System/SmartPointer.h"
#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            ManualResetEvent::ManualResetEvent(bool initialState)
                : EventWaitHandle(initialState, true) {}

            static bool s_manualDummyCreatedNew = false;
            ManualResetEvent::ManualResetEvent(const String& sName, bool initialState, bool openAlways)
                : EventWaitHandle(initialState, true, sName, openAlways, s_manualDummyCreatedNew) {}

            ManualResetEvent::ManualResetEvent(bool initialState, const String& sName, bool openAlways)
                : EventWaitHandle(initialState, true, sName, openAlways, s_manualDummyCreatedNew) {}

            ManualResetEvent::ManualResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew)
                : EventWaitHandle(initialState, true, sName, openAlways, bCreatedNew) {}

            ManualResetEvent* ManualResetEvent::OpenExisting(const String& sName) {
                ManualResetEvent* pResult = nullptr;
                if (TryOpenExisting(sName, pResult)) {
                    return pResult;
                }
                throw WaitHandleCannotBeOpenedException("No ManualResetEvent handle of the given name exists.");
            }

            bool ManualResetEvent::TryOpenExisting(const String& sName, ManualResetEvent*& pResult) {
                pResult = nullptr;
                if (sName.IsEmpty()) {
                    return false;
                }
#if defined(_WIN32)
                std::wstring wsName = Utils::StringConvert::Utf8ToWChar(sName.GetRawString());
                HANDLE h = ::OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, wsName.c_str());
                if (h != NULL) {
                    SmartPointer<ManualResetEvent> spEvt = SmartPointer<ManualResetEvent>::New(false);
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