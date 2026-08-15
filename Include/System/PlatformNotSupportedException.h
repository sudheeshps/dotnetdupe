#pragma once
#include "System/NotSupportedException.h"

namespace DotNetDupe {
    namespace System {

        class PlatformNotSupportedException : public NotSupportedException {
        public:
            PlatformNotSupportedException(const String& sMessage) : NotSupportedException(sMessage) {}
        };

    }
}
