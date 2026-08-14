#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class IOException : public SystemException {
            public:
                DOTNETDUPE_API IOException();
                IOException(const String& sMessage) : SystemException(sMessage) {}
                IOException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
            };
        }
    }
}
