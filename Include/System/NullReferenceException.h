#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        class NullReferenceException : public SystemException {
        public:
            NullReferenceException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
