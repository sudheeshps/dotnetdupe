#pragma once
#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class SocketException : public SystemException {
                private:
                    int m_errorCode;

                public:
                    DOTNETDUPE_API SocketException(int errorCode, const String& message);
                    DOTNETDUPE_API int GetErrorCode() const;
                };
            }
        }

    }
}
