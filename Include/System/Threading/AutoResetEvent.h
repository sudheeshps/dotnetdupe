#pragma once
#include "System/Threading/EventWaitHandle.h"
#include "System/SmartPointer.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class AutoResetEvent : public EventWaitHandle {
            public:
                DOTNETDUPE_API AutoResetEvent(bool initialState);
                DOTNETDUPE_API AutoResetEvent(const String& sName, bool initialState = false, bool openAlways = true);
                DOTNETDUPE_API AutoResetEvent(bool initialState, const String& sName, bool openAlways = true);
                DOTNETDUPE_API AutoResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew);

                DOTNETDUPE_API static SmartPointer<AutoResetEvent> OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<AutoResetEvent>& pResult);
            };
        }
    }
}