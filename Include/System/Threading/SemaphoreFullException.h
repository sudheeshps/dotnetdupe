#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class SemaphoreFullException : public SystemException {
            public:
                SemaphoreFullException(const String& sMessage) : SystemException(sMessage) {}
            };
        }
    }
}
