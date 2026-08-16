# WebApplication, WebAppServer &amp; ControllerBase

**Namespace:** `DotNetDupe::WebAppCore`  
**Header:** `#include "WebAppCore/Builder/WebApplication.h"`, `#include "WebAppCore/Builder/WebApplicationBuilder.h"`, `#include "WebAppCore/Controllers/ControllerBase.h"`, `#include "WebAppCore/Server/WebAppServer.h"`, `#include "WebAppCore/Http/HttpContext.h"`

An ASP.NET Core-like modern web application framework for C++ providing dependency injection bootstrapping, routing endpoints, MVC-style controllers with automated JSON serialization, static web asset serving, and real-time streaming.

---

## `WebApplicationBuilder` & `WebApplication`

### Creation & Bootstrapping
```cpp
auto builder = WebApplication::CreateBuilder();

// Register DI services
builder->GetServices().AddSingleton<IDataService, SqlDataService>();

// Register MVC controllers
builder->AddController<UsersController>("/api/users");

auto app = builder->Build();
```

### Minimal Endpoint Routing
- `MapGet(pattern, handler)`: Maps HTTP GET requests to a lambda handler.
- `MapPost(pattern, handler)`: Maps HTTP POST requests.
- `MapPut(pattern, handler)`: Maps HTTP PUT requests.
- `MapDelete(pattern, handler)`: Maps HTTP DELETE requests.
- `MapWebSocket(pattern, wsHandler)`: Maps real-time WebSocket protocol endpoints.
- `MapControllers()`: Maps all registered MVC controllers.
- `Run(url, threadCount)`: Starts the multi-threaded HTTP listener loop.
- `Stop()`: Gracefully terminates the web server.

---

## `ControllerBase`

Base class for ASP.NET Core-style controllers.

### Helper Action Results
- `Ok(model)`: Serializes `model` to JSON with HTTP `200 OK`.
- `Created(model)`: Serializes `model` to JSON with HTTP `201 Created`.
- `NoContent()`: Returns HTTP `204 NoContent`.
- `NotFound(errorMessage)`: Returns JSON error with HTTP `404 NotFound`.
- `BadRequest(errorMessage)`: Returns JSON error with HTTP `400 BadRequest`.
- `Unauthorized(errorMessage)`: Returns JSON error with HTTP `401 Unauthorized`.
- `Forbidden(errorMessage)`: Returns JSON error with HTTP `403 Forbidden`.

### Authentication & Context
- `bool Authorize(secretKey, claims)`: Validates incoming Bearer JWT token against a secret key.
- `bool Authorize(secretKey, claimName, expectedVal, claims)`: Validates token and checks required role/claim.
- `Request()`: Gets current `HttpRequest` (headers, route values, body).
- `Response()`: Gets current `HttpResponse` (headers, status code, body).

---

## `WebAppServer` (Static Web Content Hosting)

Combines Web API routing with high-performance static file serving (`index.html`, CSS, JavaScript, images) from `wwwroot`.

### Methods
- `EnableStaticFiles(defaultDocument = "index.html")`: Enables serving static files from `webRoot`.
- `SetWebRoot(directory)`: Sets target static file folder.
- `Run(url, threadCount)`: Starts the server.
- `Stop()`: Stops the server.

---

## Example

```cpp
#include "System/Console.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Controllers/ControllerBase.h"
#include "WebAppCore/Server/WebAppServer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Controllers;
using namespace DotNetDupe::WebAppCore::Server;

struct UserDto {
    int id;
    String username;
};
JSON_SERIALIZABLE(UserDto, id, username)

class ApiController : public ControllerBase {
public:
    String GetUser() {
        UserDto user { 101, "Alice" };
        return Ok(user);
    }
};

int main() {
    auto builder = WebApplication::CreateBuilder();
    builder->AddController<ApiController>("/api/v1")
           .MapGet("/user", &ApiController::GetUser);

    auto app = builder->Build();

    // Host APIs + Static Frontend
    WebAppServer server(app, "wwwroot");
    server.EnableStaticFiles("index.html");

    Console::WriteLine("Server listening at http://127.0.0.1:8080 ...");
    server.Run("http://127.0.0.1:8080", 8);

    return 0;
}
```
