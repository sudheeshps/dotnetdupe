#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class NotImplementedException : public SystemException {
        public:
            NotImplementedException(const char* pchMessage) : SystemException(pchMessage) {}
            NotImplementedException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
