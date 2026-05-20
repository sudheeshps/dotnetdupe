#pragma once
#include "System/Threading/BasicThreadInterruptedException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicThreadInterruptedException<wchar_t> ThreadInterruptedException;
#else
            typedef BasicThreadInterruptedException<char> ThreadInterruptedException;
#endif
        }
    }
}
