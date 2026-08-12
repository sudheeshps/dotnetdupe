#pragma once
#include "System/NotSupportedException.h"

namespace DotNetDupe {
    namespace System {

        class PlatformNotSupportedException : public NotSupportedException {
        public:
            PlatformNotSupportedException(const char* pchMessage) : NotSupportedException(pchMessage) {}
            PlatformNotSupportedException(const String& sMessage) : NotSupportedException(sMessage) {}
        };

    }
}
