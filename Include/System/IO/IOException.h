#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class IOException : public SystemException {
        public:
            IOException(const char* pchMessage) : SystemException(pchMessage) {}
            IOException(const String& sMessage) : SystemException(sMessage) {}
        };
        }

    }
}
