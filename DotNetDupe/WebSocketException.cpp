#include "pch.h"
#include "System/Net/WebSockets/WebSocketException.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                WebSocketException::WebSocketException(const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(WebSocketError::Faulted), m_iNativeErrorCode(0) {
                    /// Initialize: Set default Faulted error code and zero native error code.
                }

                WebSocketException::WebSocketException(WebSocketError eError, const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                    /// Initialize: Set explicit WebSocketError code.
                }

                WebSocketException::WebSocketException(WebSocketError eError, int iNativeErrorCode, const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(iNativeErrorCode) {
                    /// Initialize: Set WebSocketError code and native OS error code.
                }

                WebSocketException::WebSocketException(WebSocketError eError, const Exception& innerException)
                    : SystemException(String(innerException.What()), innerException), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                    /// Initialize: Wrap inner exception and assign WebSocketError code.
                }

                WebSocketException::WebSocketException(WebSocketError eError, const String& sMessage, const Exception& innerException)
                    : SystemException(sMessage, innerException), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                    /// Initialize: Assign message, inner exception, and WebSocketError code.
                }

                WebSocketError WebSocketException::GetWebSocketErrorCode() const {
                    /// Return result: Return structured WebSocket error code.
                    return m_eWebSocketErrorCode;
                }

                int WebSocketException::GetErrorCode() const {
                    /// Return result: Return native operating system error integer.
                    return m_iNativeErrorCode;
                }

            }
        }
    }
}
