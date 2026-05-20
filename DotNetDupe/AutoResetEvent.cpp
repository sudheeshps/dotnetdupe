#include "pch.h"
#include "System/Threading/AutoResetEvent.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            AutoResetEvent::AutoResetEvent(bool initialState) : EventWaitHandle(initialState, false) {}
        }
    }
}