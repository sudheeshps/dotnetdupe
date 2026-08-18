#include "pch.h"
#include "System/Net/WebSockets/WebSocketException.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                WebSocketException::WebSocketException(const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(WebSocketError::Faulted), m_iNativeErrorCode(0) {
                }

                WebSocketException::WebSocketException(WebSocketError eError, const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                }

                WebSocketException::WebSocketException(WebSocketError eError, int iNativeErrorCode, const String& sMessage)
                    : SystemException(sMessage), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(iNativeErrorCode) {
                }

                WebSocketException::WebSocketException(WebSocketError eError, const Exception& innerException)
                    : SystemException(String(innerException.What()), innerException), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                }

                WebSocketException::WebSocketException(WebSocketError eError, const String& sMessage, const Exception& innerException)
                    : SystemException(sMessage, innerException), m_eWebSocketErrorCode(eError), m_iNativeErrorCode(0) {
                }

                WebSocketError WebSocketException::GetWebSocketErrorCode() const {
                    return m_eWebSocketErrorCode;
                }

                int WebSocketException::GetErrorCode() const {
                    return m_iNativeErrorCode;
                }

            }
        }
    }
}
