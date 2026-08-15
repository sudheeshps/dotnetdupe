#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {

            class SecurityException : public SystemException {
            public:
                DOTNETDUPE_API SecurityException();
                SecurityException(const String& sMessage) : SystemException(sMessage) {}
                SecurityException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
            };

        }
    }
}
