#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        class OutOfMemoryException : public SystemException {
        public:
            DOTNETDUPE_API OutOfMemoryException();
            OutOfMemoryException(const String& sMessage) : SystemException(sMessage) {}
            OutOfMemoryException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}
