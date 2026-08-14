#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class InvalidOperationException : public SystemException {
        public:
            InvalidOperationException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
