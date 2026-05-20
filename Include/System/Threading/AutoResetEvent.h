#pragma once
#include "System/Threading/EventWaitHandle.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class AutoResetEvent : public EventWaitHandle {
            public:
                DOTNETDUPE_API AutoResetEvent(bool initialState);
            };
        }
    }
}