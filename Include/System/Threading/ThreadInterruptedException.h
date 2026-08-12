#pragma once
#include "System/SystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class ThreadInterruptedException : public SystemException {
            public:
                ThreadInterruptedException(const char* pchMessage);
            };
            inline ThreadInterruptedException::ThreadInterruptedException(const char* pchMessage) : SystemException(pchMessage) { }
        }
    }
}
