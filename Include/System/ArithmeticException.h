#pragma once
#include <string>
#include "System/SystemException.h"
namespace DotNetDupe {
    namespace System {
        class ArithmeticException : public SystemException {
        public:
            ArithmeticException(const char* pchMessage);
        };
        inline ArithmeticException::ArithmeticException(const char* pchMessage) : SystemException(pchMessage) { }
    }
}

