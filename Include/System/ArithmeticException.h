#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        class ArithmeticException : public SystemException {
        public:
            ArithmeticException(const String& sMessage) : SystemException(sMessage) {}
        };
    }
}
