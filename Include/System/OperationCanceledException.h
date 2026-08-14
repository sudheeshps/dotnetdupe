#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class OperationCanceledException : public SystemException {
        public:
            OperationCanceledException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
