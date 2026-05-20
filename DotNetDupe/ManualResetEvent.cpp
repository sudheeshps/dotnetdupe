#include "pch.h"
#include "System/Threading/ManualResetEvent.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            ManualResetEvent::ManualResetEvent(bool initialState) : EventWaitHandle(initialState, true) {}
        }
    }
}