#pragma once

#include "System/BasicArgumentException.h"

namespace DotNetDupe {
    namespace System {
        class ArgumentNullException : public BasicArgumentException<char> {
        public:
            ArgumentNullException(const char* message) : BasicArgumentException<char>(message) { }
        };
    }
}