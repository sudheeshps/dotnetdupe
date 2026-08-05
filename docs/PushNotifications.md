# Push Notifications & Real-Time HTTP Streaming

DotNetDupe `WebAppCore` provides real-time server-to-client push notification capabilities via **Server-Sent Events (SSE)** chunked HTTP streaming and **WebSockets**.

## Namespaces
- `DotNetDupe::WebAppCore::Http`
- `DotNetDupe::System::Net::WebSockets`
- `DotNetDupe::WebAppCore::WebSockets`

---

## 1. Server-Sent Events (SSE) Streaming API

`HttpResponse` provides methods for keeping connections open and streaming chunked data (e.g., `text/event-stream`).

### Key Methods
- `void BindStream(SmartPointer<NetworkStream> pStream)`: Binds the response directly to the underlying TCP socket network stream.
- `void FlushHeaders()`: Immediately flushes the HTTP status line and headers (e.g. `Transfer-Encoding: chunked`).
- `void WriteChunk(const String& data)`: Formats and writes a `chunked` HTTP transfer block directly to the socket.
- `void Flush()`: Flushes remaining data and sends final 0-length terminating chunk.

### Example: Server-Sent Events (SSE)

```cpp
#include "WebAppCore/Builder/WebApplication.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;

int main() {
    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    app->MapGet("/events", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> ctx) -> String {
        auto resp = ctx->GetResponse();
        resp->SetContentType("text/event-stream");

        resp->WriteChunk("data: {\"event\": \"ping\", \"time\": \"12:00:00\"}\n\n");
        resp->WriteChunk("data: {\"event\": \"metrics\", \"cpu\": 12.5}\n\n");
        resp->Flush();
        return "";
    });

    app->Run("http://127.0.0.1:5000");
    return 0;
}
```

---

## 2. WebSocket Support

Allows full-duplex persistent WebSocket connections on endpoints mapped via `MapWebSocket`.

### Key Interfaces & Classes
- **`WebSocket`**: Encapsulates framing, masking, and `SendAsync(const String& message)` push operations.
- **`IWebSocketHandler`**: Base handler class providing `OnConnected`, `OnMessage`, and `OnDisconnected` callbacks.
- **`WebSocketContext`**: Container providing access to the `HttpContext` and active `WebSocket`.

### Example: Custom WebSocket Handler

```cpp
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/WebSockets/WebSocketContext.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::WebSockets;

class NotificationsHandler : public IWebSocketHandler {
public:
    void OnConnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("[WebSocket] Client Connected!");
        pContext->GetWebSocket()->SendAsync("Welcome to Push Notifications!");
    }

    void OnMessage(SmartPointer<WebSocketContext> pContext, const String& message) override {
        Console::WriteLine(String("[WebSocket] Received: ") + message);
        pContext->GetWebSocket()->SendAsync(String("Echo: ") + message);
    }

    void OnDisconnected(SmartPointer<WebSocketContext> pContext) override {
        Console::WriteLine("[WebSocket] Client Disconnected.");
    }
};

int main() {
    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    auto handler = SmartPointer<NotificationsHandler>::NewShared();
    app->MapWebSocket("/ws/notifications", handler);

    app->Run("http://127.0.0.1:5000");
    return 0;
}
```
