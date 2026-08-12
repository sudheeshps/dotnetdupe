#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

        class WaitHandleCannotBeOpenedException : public SystemException {
        public:
            WaitHandleCannotBeOpenedException(const char* pchMessage) : SystemException(pchMessage) {}
            WaitHandleCannotBeOpenedException(const String& sMessage) : SystemException(sMessage) {}
        };
        }

    }
}
