#pragma once
#include "System/Threading/BasicAbandonedMutexException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            typedef BasicAbandonedMutexException<char> AbandonedMutexException;
        }
    }
}
