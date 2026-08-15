#pragma once
#include "System/Threading/EventWaitHandle.h"
#include "System/SmartPointer.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class ManualResetEvent : public EventWaitHandle {
            public:
                DOTNETDUPE_API ManualResetEvent(bool initialState);
                DOTNETDUPE_API ManualResetEvent(const String& sName, bool initialState = false, bool openAlways = true);
                DOTNETDUPE_API ManualResetEvent(bool initialState, const String& sName, bool openAlways = true);
                DOTNETDUPE_API ManualResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew);

                DOTNETDUPE_API static SmartPointer<ManualResetEvent> OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<ManualResetEvent>& pResult);
            };
        }
    }
}