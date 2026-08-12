#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
    namespace System {

        class ArgumentOutOfRangeException : public ArgumentException {
        public:
            ArgumentOutOfRangeException(const char* pchMessage) : ArgumentException(pchMessage) {}
            ArgumentOutOfRangeException(const String& sMessage) : ArgumentException(sMessage) {}
        };

    }
}
