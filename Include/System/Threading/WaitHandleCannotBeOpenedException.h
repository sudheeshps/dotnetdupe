#pragma once
#include "System/Threading/BasicWaitHandleCannotBeOpenedException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            typedef BasicWaitHandleCannotBeOpenedException<char> WaitHandleCannotBeOpenedException;
        }
    }
}
