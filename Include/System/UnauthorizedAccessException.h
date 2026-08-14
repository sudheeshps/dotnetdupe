#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        class UnauthorizedAccessException : public SystemException {
        public:
            DOTNETDUPE_API UnauthorizedAccessException();
            UnauthorizedAccessException(const String& sMessage) : SystemException(sMessage) {}
            UnauthorizedAccessException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}
