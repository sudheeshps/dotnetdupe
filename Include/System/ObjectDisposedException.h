#pragma once
#include "System/InvalidOperationException.h"

namespace DotNetDupe {
    namespace System {

        class ObjectDisposedException : public InvalidOperationException {
        public:
            ObjectDisposedException(const char* pchMessage) : InvalidOperationException(pchMessage) {}
            ObjectDisposedException(const String& sMessage) : InvalidOperationException(sMessage) {}
        };

    }
}
