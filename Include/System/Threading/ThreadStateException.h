#pragma once
#include "System/Threading/BasicThreadStateException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicThreadStateException<wchar_t> ThreadStateException;
#else
            typedef BasicThreadStateException<char> ThreadStateException;
#endif
        }
    }
}
