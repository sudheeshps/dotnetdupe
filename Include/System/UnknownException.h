#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        class UnknownException : public SystemException {
        public:
            DOTNETDUPE_API UnknownException();
            DOTNETDUPE_API UnknownException(const String& sMessage) : SystemException(sMessage) {}
            DOTNETDUPE_API UnknownException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}
