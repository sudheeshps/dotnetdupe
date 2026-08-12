#pragma once
#include "System/ArgumentException.h"

namespace DotNetDupe {
    namespace System {

        class ArgumentNullException : public ArgumentException {
        public:
            ArgumentNullException(const char* pchMessage) : ArgumentException(pchMessage) {}
            ArgumentNullException(const String& sMessage) : ArgumentException(sMessage) {}
        };

    }
}
