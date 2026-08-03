#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Threading/Thread.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/WebSockets/WebSocketContext.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::WebSockets;

class DemoPushHandler : public IWebSocketHandler {
public:
    void OnConnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("[DemoWebSocket] Client Connected!");
        pContext->GetWebSocket()->SendAsync("Welcome to Push Notifications!");
    }

    void OnMessage(SmartPointer<WebSocketContext> pContext, const String& message) override {
        Console::WriteLine(String("[DemoWebSocket] Received: ") + message);
        pContext->GetWebSocket()->SendAsync(String("Echo Push: ") + message);
    }

    void OnDisconnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("[DemoWebSocket] Client Disconnected.");
    }
};

void DemonstratePushNotifications() {
    Console::WriteLine("\n--- Push Notifications & Streaming Demonstration ---");

    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    app->MapGet("/events", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> ctx) -> String {
        auto resp = ctx->GetResponse();
        resp->SetContentType("text/event-stream");
        resp->WriteChunk("data: {\"type\": \"alert\", \"msg\": \"Server Push Event 1\"}\n\n");
        resp->WriteChunk("data: {\"type\": \"alert\", \"msg\": \"Server Push Event 2\"}\n\n");
        resp->Flush();
        return "";
    });

    auto wsHandler = SmartPointer<DemoPushHandler>::NewShared();
    app->MapWebSocket("/ws/push", wsHandler);

    Console::WriteLine("Configured Push Notification routes:");
    Console::WriteLine(" - GET  /events   (SSE Chunked HTTP Streaming)");
    Console::WriteLine(" - WS   /ws/push  (WebSocket Full-Duplex Push)");
    Console::WriteLine("Push Notifications setup complete.");
}
