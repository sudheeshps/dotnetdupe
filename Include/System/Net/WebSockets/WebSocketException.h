#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                enum class WebSocketError {
                    Success = 0,
                    InvalidMessageType = 1,
                    Faulted = 2,
                    NativeError = 3,
                    NotAWebSocket = 4,
                    UnsupportedVersion = 5,
                    UnsupportedProtocol = 6,
                    HeaderError = 7,
                    ConnectionClosedPrematurely = 8,
                    InvalidState = 9
                };

                class WebSocketException : public SystemException {
                private:
                    WebSocketError m_eWebSocketErrorCode;
                    int m_iNativeErrorCode;

                public:
                    DOTNETDUPE_API explicit WebSocketException(const String& sMessage);
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const String& sMessage);
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, int iNativeErrorCode, const String& sMessage);
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const Exception& innerException);
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const String& sMessage, const Exception& innerException);

                    DOTNETDUPE_API WebSocketError GetWebSocketErrorCode() const;
                    DOTNETDUPE_API int GetErrorCode() const;
                };

            }
        }
    }
}
