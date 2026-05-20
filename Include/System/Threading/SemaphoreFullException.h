#pragma once
#include "System/Threading/BasicSemaphoreFullException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicSemaphoreFullException<wchar_t> SemaphoreFullException;
#else
            typedef BasicSemaphoreFullException<char> SemaphoreFullException;
#endif
        }
    }
}
