#pragma once
#include "System/SystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class SemaphoreFullException : public SystemException {
            public:
                SemaphoreFullException(const char* pchMessage);
            };
            inline SemaphoreFullException::SemaphoreFullException(const char* pchMessage) : SystemException(pchMessage) { }
        }
    }
}
