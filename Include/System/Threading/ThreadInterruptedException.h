#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class ThreadInterruptedException : public SystemException {
            public:
                ThreadInterruptedException(const String& sMessage) : SystemException(sMessage) {}
            };
        }
    }
}
