#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

        class SocketException : public SystemException {
        public:
            SocketException(const char* pchMessage) : SystemException(pchMessage) {}
            SocketException(const String& sMessage) : SystemException(sMessage) {}
        };
            }
        }

    }
}
