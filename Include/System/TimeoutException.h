#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class TimeoutException : public SystemException {
        public:
            TimeoutException(const char* pchMessage) : SystemException(pchMessage) {}
            TimeoutException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
