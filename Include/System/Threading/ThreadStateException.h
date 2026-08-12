#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

        class ThreadStateException : public SystemException {
        public:
            ThreadStateException(const char* pchMessage) : SystemException(pchMessage) {}
            ThreadStateException(const String& sMessage) : SystemException(sMessage) {}
        };
        }

    }
}
