#pragma once
#include "System/Threading/BasicAbandonedMutexException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
#ifdef UNICODE
            typedef BasicAbandonedMutexException<wchar_t> AbandonedMutexException;
#else
            typedef BasicAbandonedMutexException<char> AbandonedMutexException;
#endif
        }
    }
}
