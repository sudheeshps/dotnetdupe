#pragma once
#include "System/Threading/BasicWaitHandleCannotBeOpenedException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicWaitHandleCannotBeOpenedException<wchar_t> WaitHandleCannotBeOpenedException;
#else
            typedef BasicWaitHandleCannotBeOpenedException<char> WaitHandleCannotBeOpenedException;
#endif
        }
    }
}
