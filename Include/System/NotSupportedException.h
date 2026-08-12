#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class NotSupportedException : public SystemException {
        public:
            NotSupportedException(const char* pchMessage) : SystemException(pchMessage) {}
            NotSupportedException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
