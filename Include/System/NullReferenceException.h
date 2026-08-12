#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class NullReferenceException : public SystemException {
        public:
            NullReferenceException(const char* pchMessage) : SystemException(pchMessage) {}
            NullReferenceException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
