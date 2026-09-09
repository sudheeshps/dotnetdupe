/// \file WebSocket.h
/// \brief Represents an RFC 6455 full-duplex WebSocket connection over a NetworkStream.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Array.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                /// \brief Defines the operational states that a WebSocket connection can be in.
                /// \note Conforms to RFC 6455 Section 4.1 connection states.
                enum class WebSocketState {
                    None = 0,            ///< Initial state before connection setup.
                    Connecting = 1,      ///< The connection is negotiating the handshake.
                    Open = 2,            ///< The connection is open and ready for sending/receiving frames.
                    CloseSent = 3,       ///< A close control frame was transmitted.
                    CloseReceived = 4,   ///< A close control frame was received from the remote peer.
                    Closed = 5,          ///< The connection has cleanly closed.
                    Aborted = 6          ///< The connection was aborted or closed abruptly.
                };

                /// \brief Manages full-duplex message framing and communication over an established WebSocket connection.
                ///
                /// Encapsulates RFC 6455 binary and text framing, payload masking/unmasking, extended length parsing,
                /// SHA-1/Base64 handshake calculation, and thread-safe sending.
                ///
                /// \note Thread Safety: Thread-safe; internal synchronization protects frame transmission and state transitions.
                /// \note Conforms to RFC 6455 (The WebSocket Protocol).
                /// \see WebSocketException, WebSocketState, NetworkStream
                class WebSocket : public virtual Object {
                public:
                    /// \brief Constructs a WebSocket instance bound to the specified open network stream.
                    /// \param pStream SmartPointer to the active NetworkStream.
                    DOTNETDUPE_API explicit WebSocket(SmartPointer<Sockets::NetworkStream> pStream);

                    /// \brief Destructor for WebSocket.
                    DOTNETDUPE_API ~WebSocket() override;

                    /// \brief Gets the current operational state of the WebSocket.
                    /// \return Active WebSocketState value.
                    DOTNETDUPE_API WebSocketState GetState() const;

                    /// \brief Sets the operational state of the WebSocket.
                    /// \param state New WebSocketState to assign.
                    DOTNETDUPE_API void SetState(WebSocketState state);

                    /// \brief Sends a complete UTF-8 text message frame (opcode 0x1) to the remote peer.
                    /// \param message UTF-8 text string to transmit.
                    /// \return True if the frame was successfully written; otherwise, false.
                    /// \throws WebSocketException if the socket state is invalid or transmission fails.
                    DOTNETDUPE_API bool SendAsync(const String& message);

                    /// \brief Sends a complete binary message frame (opcode 0x2) to the remote peer.
                    /// \param data Byte array payload to transmit.
                    /// \return True if the frame was successfully written; otherwise, false.
                    /// \throws WebSocketException if the socket state is invalid or transmission fails.
                    DOTNETDUPE_API bool SendBytes(const Array<uint8_t>& data);

                    /// \brief Receives and decodes the next complete text message frame from the stream.
                    /// \param outMessage Output string populated with the unmasked text payload.
                    /// \return True if a valid text frame was received; false if EOF or close frame was encountered.
                    /// \throws WebSocketException on protocol violation, frame decoding error, or unexpected disconnection.
                    DOTNETDUPE_API bool ReceiveText(String& outMessage);

                    /// \brief Transmits an RFC 6455 close frame (opcode 0x8) and marks the state as Closed.
                    DOTNETDUPE_API void Close();

                    /// \brief Computes the RFC 6455 Sec-WebSocket-Accept handshake header from a client challenge key.
                    /// \param secWebSocketKey Client-provided 16-byte nonce encoded in Base64.
                    /// \return Base64-encoded SHA-1 hash concatenated with the RFC 6455 magic GUID string.
                    /// \throws ArgumentException if secWebSocketKey is empty.
                    DOTNETDUPE_API static String ComputeSecWebSocketAccept(const String& secWebSocketKey);

                private:
                    class Impl;
                    SmartPointer<Impl> m_pImpl;
                };

            }
        }
    }
}
