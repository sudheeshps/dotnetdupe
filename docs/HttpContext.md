# HttpContext, HttpRequest & HttpResponse

**Namespace:** `DotNetDupe::WebAppCore::Http`  
**Header:** `#include "WebAppCore/Http/HttpContext.h"`

Encapsulates all HTTP-specific context regarding an individual HTTP request, including request verbs, headers, queries, route values, and response output streaming (supporting chunked transfer and Server-Sent Events).

---

## Syntax

```cpp
class HttpRequest : public virtual DotNetDupe::System::Object;
class HttpResponse : public virtual DotNetDupe::System::Object;
class HttpContext : public virtual DotNetDupe::System::Object;
```

---

## HttpRequest Members

- `String GetMethod() const` / `void SetMethod(const String& method)`: HTTP method (GET, POST, etc.).
- `String GetPath() const` / `void SetPath(const String& path)`: Request URL path.
- `String GetBody() const` / `void SetBody(const String& body)`: Request payload string.
- `Dictionary<String, String>& GetHeaders()`: Request headers map.
- `Dictionary<String, String>& GetQuery()`: Parsed query string parameters (`?key=val`).
- `Dictionary<String, String>& GetRouteValues()`: Mapped route template parameters.

---

## HttpResponse Members

- `int GetStatusCode() const` / `void SetStatusCode(int code)`: HTTP response status.
- `String GetContentType() const` / `void SetContentType(const String& type)`: Content-Type header.
- `void Write(const String& text)`: Appends text to response body.
- `void FlushHeaders()`: Sends HTTP status line and headers immediately.
- `void WriteChunk(const String& data)`: Sends chunked transfer data chunk (e.g. for streaming SSE).
- `void Flush()`: Flushes underlying network socket stream.

---

## HttpContext Members

- `SmartPointer<HttpRequest> GetRequest() const`: Gets incoming request.
- `SmartPointer<HttpResponse> GetResponse() const`: Gets outgoing response.

---

## Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "WebAppCore/Http/HttpContext.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Http;

void HandleIncomingRequest(SmartPointer<HttpContext> ctx) {
    auto req = ctx->GetRequest();
    auto res = ctx->GetResponse();

    Console::WriteLine("Received {0} {1}", req->GetMethod(), req->GetPath());

    if (req->GetQuery().ContainsKey("format")) {
        res->SetContentType("application/json");
        res->SetBody("{\"status\":\"ok\"}");
    } else {
        res->SetContentType("text/plain");
        res->SetBody("OK");
    }
}
```
