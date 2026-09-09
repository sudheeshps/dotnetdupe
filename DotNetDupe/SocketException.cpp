#include "pch.h"
#include "System/Net/Sockets/SocketException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {
                SocketException::SocketException(int errorCode, const String& message)
                    : SystemException(message) {
                    /// Step: Record native socket error code.
                    m_errorCode = errorCode;
                }

                int SocketException::GetErrorCode() const {
                    /// Return: Native socket error code.
                    return m_errorCode;
                }
            }
        }
    }
}
