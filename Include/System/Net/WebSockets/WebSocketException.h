/// \file WebSocketException.h
/// \brief Exception thrown when an error occurs during RFC 6455 WebSocket communication or handshakes.

#pragma once

#include "Common.h"
#include "System/String.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                /// \brief Categorizes errors that occur during WebSocket protocol handshakes, framing, or socket I/O.
                /// \note Conforms to RFC 6455 error definitions.
                enum class WebSocketError {
                    Success = 0,                     ///< No error occurred.
                    InvalidMessageType = 1,          ///< Received frame has an unknown or unexpected opcode.
                    Faulted = 2,                     ///< General fault occurred during processing.
                    NativeError = 3,                 ///< Underlying socket or OS network error.
                    NotAWebSocket = 4,               ///< The connection or payload does not conform to RFC 6455.
                    UnsupportedVersion = 5,          ///< Requested Sec-WebSocket-Version is not supported.
                    UnsupportedProtocol = 6,         ///< Requested subprotocol is not supported.
                    HeaderError = 7,                 ///< Malformed WebSocket frame header or HTTP upgrade header.
                    ConnectionClosedPrematurely = 8,  ///< Connection closed before frame payload was completely read.
                    InvalidState = 9                 ///< Attempted operation on an inactive, closed, or aborted socket.
                };

                /// \brief Represents errors that occur during WebSocket connection operations and message framing.
                ///
                /// Inherits from DotNetDupe::System::SystemException and encapsulates structured WebSocketError codes
                /// alongside platform-specific error numbers.
                ///
                /// \note Conforms to RFC 6455 (The WebSocket Protocol).
                /// \see WebSocket, WebSocketError, SystemException
                class WebSocketException : public SystemException {
                private:
                    WebSocketError m_eWebSocketErrorCode;
                    int m_iNativeErrorCode;

                public:
                    /// \brief Constructs a WebSocketException with a custom descriptive error message.
                    /// \param sMessage Descriptive error message string.
                    DOTNETDUPE_API explicit WebSocketException(const String& sMessage);

                    /// \brief Constructs a WebSocketException with a specific WebSocketError code and message.
                    /// \param eError WebSocketError code identifying the failure type.
                    /// \param sMessage Descriptive error message string.
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const String& sMessage);

                    /// \brief Constructs a WebSocketException with error code, native OS error code, and message.
                    /// \param eError WebSocketError code identifying the failure type.
                    /// \param iNativeErrorCode Operating system or socket error code.
                    /// \param sMessage Descriptive error message string.
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, int iNativeErrorCode, const String& sMessage);

                    /// \brief Constructs a WebSocketException wrapping an underlying inner exception.
                    /// \param eError WebSocketError code identifying the failure type.
                    /// \param innerException Inner Exception causing this failure.
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const Exception& innerException);

                    /// \brief Constructs a WebSocketException with error code, message, and inner exception.
                    /// \param eError WebSocketError code identifying the failure type.
                    /// \param sMessage Descriptive error message string.
                    /// \param innerException Inner Exception causing this failure.
                    DOTNETDUPE_API WebSocketException(WebSocketError eError, const String& sMessage, const Exception& innerException);

                    /// \brief Gets the structured WebSocketError enumeration code.
                    /// \return Active WebSocketError value.
                    DOTNETDUPE_API WebSocketError GetWebSocketErrorCode() const;

                    /// \brief Gets the native operating system error code associated with this exception.
                    /// \return Native error integer (e.g., errno or WinSock error code).
                    DOTNETDUPE_API int GetErrorCode() const;
                };

            }
        }
    }
}
