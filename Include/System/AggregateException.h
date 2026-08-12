#pragma once
#include "System/Exception.h"

namespace DotNetDupe {
    namespace System {

        class AggregateException : public Exception {
        public:
            AggregateException(const char* pchMessage) : Exception(pchMessage) {}
            AggregateException(const String& sMessage) : Exception(sMessage) {}
        };

    }
}
