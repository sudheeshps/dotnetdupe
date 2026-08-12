#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class ArgumentException : public SystemException {
        public:
            ArgumentException(const char* pchMessage) : SystemException(pchMessage) {}
            ArgumentException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
