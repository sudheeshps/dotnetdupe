#pragma once
#include "System/InvalidOperationException.h"

namespace DotNetDupe {
    namespace System {

        class ObjectDisposedException : public InvalidOperationException {
        public:
            ObjectDisposedException(const String& sMessage) : InvalidOperationException(sMessage) {}
        };

    }
}
