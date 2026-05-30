#pragma once
#include "System/Threading/BasicSemaphoreFullException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            typedef BasicSemaphoreFullException<char> SemaphoreFullException;
        }
    }
}
