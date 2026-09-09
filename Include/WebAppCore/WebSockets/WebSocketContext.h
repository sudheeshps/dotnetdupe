/// \file WebSocketContext.h
/// \brief Context and handler abstractions for WebSockets integrated into the WebAppCore HTTP server pipeline.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Action.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "WebAppCore/Http/HttpContext.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace WebSockets {

            /// \brief Encapsulates the runtime context of an active WebSocket connection within the WebAppCore server.
            ///
            /// Provides paired access to the upgraded HttpContext (including handshake headers, authentication claims,
            /// and query parameters) and the underlying bidirectional WebSocket instance.
            ///
            /// \note Thread Safety: Safe for reading context properties; socket I/O is synchronized internally by WebSocket.
            /// \note Conforms to RFC 6455 (The WebSocket Protocol).
            /// \see IWebSocketHandler, HttpContext, WebSocket
            class WebSocketContext : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Constructs a WebSocketContext wrapping the originating HttpContext and active WebSocket.
                /// \param pContext SmartPointer to the originating HttpContext.
                /// \param pWebSocket SmartPointer to the established WebSocket.
                DOTNETDUPE_API WebSocketContext(DotNetDupe::System::SmartPointer<Http::HttpContext> pContext,
                                                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> pWebSocket)
                    : m_pContext(pContext), m_pWebSocket(pWebSocket) {}

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~WebSocketContext() override = default;

                /// \brief Gets the originating HTTP upgrade context.
                /// \return SmartPointer to HttpContext.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<Http::HttpContext> GetHttpContext() const { return m_pContext; }

                /// \brief Gets the active bidirectional WebSocket connection.
                /// \return SmartPointer to WebSocket.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> GetWebSocket() const { return m_pWebSocket; }

            private:
                DotNetDupe::System::SmartPointer<Http::HttpContext> m_pContext;
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> m_pWebSocket;
            };

            /// \brief Interface for receiving WebSocket connection lifecycle events and incoming messages.
            ///
            /// Applications implement this interface to process asynchronous client connections, frame messages,
            /// and clean disconnection signals.
            ///
            /// \note Thread Safety: Handlers must ensure thread safety if shared state is mutated across concurrent connections.
            /// \note Conforms to RFC 6455 (The WebSocket Protocol).
            /// \see WebSocketContext, WebSocket
            class IWebSocketHandler : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Virtual destructor.
                DOTNETDUPE_API ~IWebSocketHandler() override = default;

                /// \brief Invoked immediately after a successful RFC 6455 WebSocket handshake.
                /// \param pContext SmartPointer to the connection WebSocketContext.
                DOTNETDUPE_API virtual void OnConnected(DotNetDupe::System::SmartPointer<WebSocketContext> pContext) = 0;

                /// \brief Invoked when a complete text message frame is received from the remote client.
                /// \param pContext SmartPointer to the connection WebSocketContext.
                /// \param message Received UTF-8 text message payload.
                DOTNETDUPE_API virtual void OnMessage(DotNetDupe::System::SmartPointer<WebSocketContext> pContext, const DotNetDupe::System::String& message) = 0;

                /// \brief Invoked when the WebSocket connection is closed or the remote peer disconnects.
                /// \param pContext SmartPointer to the connection WebSocketContext.
                DOTNETDUPE_API virtual void OnDisconnected(DotNetDupe::System::SmartPointer<WebSocketContext> pContext) = 0;
            };

        }
    }
}
