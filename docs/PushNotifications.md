# Push Notifications (SSE &amp; WebSockets)

**Namespace:** `DotNetDupe::WebAppCore::WebSockets` & `DotNetDupe::WebAppCore::Http`  
**Header:** `#include "WebAppCore/WebSockets/WebSocketContext.h"`, `#include "WebAppCore/Http/HttpContext.h"`

Provides real-time push notification streaming from the C++ backend to connected browser clients using either Server-Sent Events (SSE) or full-duplex WebSockets.

---

## 1. Server-Sent Events (SSE)

SSE enables unidirectional push event streams over standard HTTP chunked transfer encoding.

### SSE Endpoint Implementation
```cpp
app->MapGet("/api/live-feed", [](SmartPointer<HttpContext> context) -> String {
    auto response = context->GetResponse();
    response->SetContentType("text/event-stream");
    response->GetHeaders().Add("Cache-Control", "no-cache");
    response->GetHeaders().Add("Connection", "keep-alive");
    response->SetChunked(true);
    response->FlushHeaders();

    for (int i = 1; i <= 5; ++i) {
        String data = String::Format("data: {\"event\": \"update\", \"count\": {0}}\n\n", i);
        response->WriteChunk(data);
        Thread::Sleep(1000);
    }

    return "";
});
```

---

## 2. WebSockets (`IWebSocketHandler`)

WebSockets provide low-latency, full-duplex bidirectional communication channels.

### `IWebSocketHandler` Interface
```cpp
class IWebSocketHandler : public virtual Object {
public:
    virtual void OnConnected(SmartPointer<WebSocketContext> pContext) = 0;
    virtual void OnMessage(SmartPointer<WebSocketContext> pContext, const String& message) = 0;
    virtual void OnDisconnected(SmartPointer<WebSocketContext> pContext) = 0;
};
```

### Mapping WebSocket Handlers
```cpp
class ChatSocketHandler : public IWebSocketHandler {
public:
    void OnConnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("Client connected via WebSocket!");
    }

    void OnMessage(SmartPointer<WebSocketContext> pContext, const String& message) override {
        Console::WriteLine("Received: {0}", message);
        pContext->GetWebSocket()->Send("Echo: " + message);
    }

    void OnDisconnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("Client disconnected.");
    }
};

// Map on WebApplication
app->MapWebSocket("/ws/chat", SmartPointer<ChatSocketHandler>::NewShared());
```

---

## Example

```cpp
#include "System/Console.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/WebSockets/WebSocketContext.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Http;
using namespace DotNetDupe::WebAppCore::WebSockets;

int main() {
    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    // SSE Endpoint
    app->MapGet("/events", [](SmartPointer<HttpContext> ctx) -> String {
        auto res = ctx->GetResponse();
        res->SetContentType("text/event-stream");
        res->SetChunked(true);
        res->FlushHeaders();

        res->WriteChunk("data: Hello from SSE\n\n");
        return "";
    });

    Console::WriteLine("Streaming server running at http://127.0.0.1:8080/events");
    app->Run("http://127.0.0.1:8080", 4);

    return 0;
}
```
