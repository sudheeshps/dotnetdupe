#pragma once
#include "System/Threading/BasicSynchronizationLockException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicSynchronizationLockException<wchar_t> SynchronizationLockException;
#else
            typedef BasicSynchronizationLockException<char> SynchronizationLockException;
#endif
        }
    }
}
