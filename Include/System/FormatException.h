#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class FormatException : public SystemException {
        public:
            FormatException(const char* pchMessage) : SystemException(pchMessage) {}
            FormatException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
