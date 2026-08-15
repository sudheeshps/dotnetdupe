#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                class JsonException : public SystemException {
                public:
                    DOTNETDUPE_API JsonException() : SystemException(String("A JSON parse error occurred.")) {}
                    DOTNETDUPE_API JsonException(const String& sMessage) : SystemException(sMessage) {}
                    DOTNETDUPE_API JsonException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
                };
            }
        }
    }
}
