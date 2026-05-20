#pragma once
#include "System/Threading/EventWaitHandle.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class ManualResetEvent : public EventWaitHandle {
            public:
                DOTNETDUPE_API ManualResetEvent(bool initialState);
            };
        }
    }
}