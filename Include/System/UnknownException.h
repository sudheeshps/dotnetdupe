#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        class UnknownException : public SystemException {
        public:
            DOTNETDUPE_API UnknownException();
            UnknownException(const String& sMessage) : SystemException(sMessage) {}
            UnknownException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}
