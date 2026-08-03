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

            class WebSocketContext : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API WebSocketContext(DotNetDupe::System::SmartPointer<Http::HttpContext> pContext,
                                                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> pWebSocket)
                    : m_pContext(pContext), m_pWebSocket(pWebSocket) {}
                DOTNETDUPE_API ~WebSocketContext() override = default;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<Http::HttpContext> GetHttpContext() const { return m_pContext; }
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> GetWebSocket() const { return m_pWebSocket; }

            private:
                DotNetDupe::System::SmartPointer<Http::HttpContext> m_pContext;
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::WebSockets::WebSocket> m_pWebSocket;
            };

            class IWebSocketHandler : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API ~IWebSocketHandler() override = default;
                DOTNETDUPE_API virtual void OnConnected(DotNetDupe::System::SmartPointer<WebSocketContext> pContext) = 0;
                DOTNETDUPE_API virtual void OnMessage(DotNetDupe::System::SmartPointer<WebSocketContext> pContext, const DotNetDupe::System::String& message) = 0;
                DOTNETDUPE_API virtual void OnDisconnected(DotNetDupe::System::SmartPointer<WebSocketContext> pContext) = 0;
            };

        }
    }
}
