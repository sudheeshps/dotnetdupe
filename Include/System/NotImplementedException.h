#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class NotImplementedException : public SystemException {
        public:
            NotImplementedException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
