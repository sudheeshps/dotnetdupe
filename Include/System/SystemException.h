#pragma once
#include "System/Exception.h"

namespace DotNetDupe {
    namespace System {
        class SystemException : public Exception {
        public:
            SystemException(const char* pchMessage) : Exception(pchMessage) {}
            SystemException(const String& sMessage) : Exception(sMessage) {}
        };
    }
}
