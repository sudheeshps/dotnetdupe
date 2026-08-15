#include "pch.h"
#include "System/Net/Sockets/SocketException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {
                SocketException::SocketException(int errorCode, const String& message)
                    : SystemException(message) {
                    m_errorCode = errorCode;
                }

                int SocketException::GetErrorCode() const {
                    return m_errorCode;
                }
            }
        }
    }
}
