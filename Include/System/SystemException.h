#pragma once
#include "System/Exception.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        class SystemException : public Exception {
        public:
            DOTNETDUPE_API SystemException();
            DOTNETDUPE_API SystemException(const char* pchMessage);
            DOTNETDUPE_API SystemException(const String& sMessage);
            DOTNETDUPE_API SystemException(const String& sMessage, const Exception& innerException);
        };
    }
}
