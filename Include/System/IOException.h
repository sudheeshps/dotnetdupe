#pragma once
#include <string>
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class IOException : public SystemException {
            public:
                DOTNETDUPE_API IOException() : SystemException(String("I/O error occurred.")) {}
                DOTNETDUPE_API IOException(const String& sMessage) : SystemException(sMessage) {}
                DOTNETDUPE_API IOException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
            };
        }
    }
}
