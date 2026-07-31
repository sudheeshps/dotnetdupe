# WebAppServer

`DotNetDupe::WebAppCore::Server::WebAppServer` provides static file serving capabilities (e.g. `index.html`, CSS, JavaScript, images, fonts, icons) alongside ASP.NET Core-style `WebApplication` REST API endpoints and controllers.

## Namespace
`DotNetDupe::WebAppCore::Server`

## Header
```cpp
#include "WebAppCore/Server/WebAppServer.h"
```

## Key Methods

- **`WebAppServer(const SmartPointer<WebApplication>& app, const String& webRoot = "wwwroot")`**: Initializes a web server hosting the specified `WebApplication` and root directory.
- **`void EnableStaticFiles(const String& defaultDocument = "index.html")`**: Configures static file fallback matching with a default document index.
- **`void SetWebRoot(const String& webRoot)`**: Configures the directory path serving static assets.
- **`String GetWebRoot() const`**: Returns the configured web root directory.
- **`static String GetMimeType(const String& filePath)`**: Resolves MIME content types based on file extension.
- **`void Run(const String& url = "http://127.0.0.1:5000")`**: Starts listening for incoming HTTP requests.
- **`void Stop()`**: Stops the server loop.

---

## Supported MIME Types
- `.html`, `.htm` -> `text/html; charset=utf-8`
- `.css` -> `text/css; charset=utf-8`
- `.js` -> `application/javascript; charset=utf-8`
- `.json` -> `application/json; charset=utf-8`
- `.png` -> `image/png`
- `.jpg`, `.jpeg` -> `image/jpeg`
- `.gif` -> `image/gif`
- `.svg` -> `image/svg+xml`
- `.ico` -> `image/x-icon`
- `.woff`, `.woff2` -> `font/woff`, `font/woff2`

---

## Complete Usage Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Server/WebAppServer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Server;

int main() {
    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    // Map REST API endpoint
    app->MapGet("/api/status", [](SmartPointer<Http::HttpContext> ctx) {
        return String("{\"status\":\"Online\"}");
    });

    // Create WebAppServer hosting 'wwwroot' folder
    WebAppServer server(app, "wwwroot");
    server.EnableStaticFiles("index.html");

    // Starts serving static files (index.html, images, CSS) and API endpoints
    server.Run("http://127.0.0.1:5000");

    return 0;
}
```
