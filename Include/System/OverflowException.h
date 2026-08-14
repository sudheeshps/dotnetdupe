#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class OverflowException : public SystemException {
        public:
            OverflowException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
