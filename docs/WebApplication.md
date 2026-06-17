### Web Application (Minimal APIs)

Provides a lightweight HTTP web server host and request-response pipeline model, mimicking ASP.NET Core's minimal hosting APIs. It allows developers to register dependencies, configure routing endpoints (GET, POST, etc.), extract headers, query parameters, or bodies from HTTP requests, and send structured responses.

#### Namespaces
- `DotNetDupe::WebAppCore::Builder` (for `WebApplication` and `WebApplicationBuilder`)
- `DotNetDupe::WebAppCore::Http` (for `HttpContext`, `HttpRequest`, and `HttpResponse`)

---

### Key Classes

#### `class WebApplicationBuilder`
Used to configure application services and build the web application host.

##### Methods
- `Extensions::DependencyInjection::ServiceCollection& GetServices()`: Gets the DI service collection for registering application dependencies.
- `template <typename TController> WebAppCore::Controllers::ControllerRouteBuilder<TController>& AddController(const String& prefix)`: Registers the controller in the DI service collection and returns a reference to its route builder for fluent mapping.
- `SmartPointer<WebApplication> Build()`: Compiles the service provider and returns the configured `WebApplication` instance, transferring any queued controller routes.

---

#### `class WebApplication`
The central HTTP server host. Handles endpoint mapping and listens for incoming connections.

##### Methods
- `static SmartPointer<WebApplicationBuilder> CreateBuilder()`: Initializes a new builder instance.
- `SmartPointer<IServiceProvider> GetServices() const`: Gets the built dependency injection provider.
- `void MapGet(const String& pattern, Func<String, SmartPointer<Http::HttpContext>> handler)`: Maps a GET request endpoint to a route handler. Supports path-parameter templates like `/api/users/{id}`.
- `void MapPost(const String& pattern, Func<String, SmartPointer<Http::HttpContext>> handler)`: Maps a POST request endpoint to a route handler.
- `void MapPut(const String& pattern, Func<String, SmartPointer<Http::HttpContext>> handler)`: Maps a PUT request endpoint to a route handler. Supports path-parameter templates like `/api/users/{id}`.
- `void MapDelete(const String& pattern, Func<String, SmartPointer<Http::HttpContext>> handler)`: Maps a DELETE request endpoint to a route handler. Supports path-parameter templates like `/api/users/{id}`.
- `void MapControllers()`: Iterates through all registered controllers and hooks up their route callbacks automatically.
- `void Run(const String& url = "http://127.0.0.1:5000")`: Starts listening on the specified URL (blocking loop).
- `void Stop()`: Halts the web server.

---

#### `class HttpContext`
Encapsulates all HTTP-specific information about an individual HTTP request.

##### Methods
- `SmartPointer<HttpRequest> GetRequest() const`: Returns the incoming HTTP request data.
- `SmartPointer<HttpResponse> GetResponse() const`: Returns the outgoing HTTP response configuration.

---

#### `class HttpRequest`
Represents the incoming side of an HTTP request.

##### Methods
- `String GetMethod() const`: Gets the HTTP method (e.g. `"GET"`, `"POST"`, `"PUT"`, `"DELETE"`).
- `String GetPath() const`: Gets the request path (e.g. `"/hello"`).
- `String GetBody() const`: Gets the raw request body.
- `Dictionary<String, String>& GetHeaders()`: Returns the HTTP headers dictionary (casing is converted to lowercase during parsing).
- `Dictionary<String, String>& GetQuery()`: Returns parsed query string parameters.
- `Dictionary<String, String>& GetRouteValues()`: Returns the route parameter values extracted from path parameter templates.

---

#### `class HttpResponse`
Represents the outgoing side of an HTTP response.

##### Methods
- `int GetStatusCode() const`: Gets the status code (default: `200`).
- `void SetStatusCode(int code)`: Sets the HTTP status code (e.g. `200`, `201`, `404`).
- `String GetContentType() const`: Gets the media type (default: `"text/plain"`).
- `void SetContentType(const String& type)`: Sets the media type (e.g. `"application/json"`).
- `void Write(const String& text)`: Appends text to the response body.
- `Dictionary<String, String>& GetHeaders()`: Allows setting custom outgoing HTTP headers.

---

#### `class ControllerBase`
Base class for MVC/Web API style controllers, providing access to HttpContext and helpers for standard HTTP responses.

##### Methods
- `void Initialize(const SmartPointer<HttpContext>& context)`: Binds the HTTP context to the controller.
- `SmartPointer<HttpRequest> Request() const`: Returns the incoming HTTP request.
- `SmartPointer<HttpResponse> Response() const`: Returns the outgoing HTTP response.
- `template <typename U> String Ok(const U& value)`: Automatically serializes the C++ object/collection `value` to JSON, sets `Content-Type: application/json`, and returns it with status 200 OK.
- `String Ok(const String& body)`: Returns status 200 OK with raw body.
- `template <typename U> String Created(const U& value)`: Automatically serializes the C++ object/collection `value` to JSON, sets `Content-Type: application/json`, and returns it with status 201 Created.
- `String Created(const String& body)`: Returns status 201 Created with raw body.
- `String NoContent()`: Returns status 204 No Content.
- `String NotFound(const String& error)`: Returns status 404 Not Found with JSON error payload.
- `String BadRequest(const String& error)`: Returns status 400 Bad Request with JSON error payload.
- `String Unauthorized(const String& error)`: Returns status 401 Unauthorized with JSON error payload.

---

#### `template <typename TController> class ControllerRouteBuilder`
A templated mapping registrar that maps controller member functions (actions) to WebApplication routes, automating JSON serialization and deserialization and supporting fluent route registration. It allows action methods to return strongly typed resources directly (e.g., custom structures, `List<T>`) which are automatically serialized to JSON.

##### Methods
- `ControllerRouteBuilder(const String& prefix)`: Binds a builder to a specific route prefix (e.g. `/api/users`).
- `ControllerRouteBuilder& MapGet(const String& subPath, String (TController::*action)())`: Maps a GET request returning a raw string or response helper.
- `template <typename TResult> ControllerRouteBuilder& MapGet(const String& subPath, TResult (TController::*action)())`: Maps a GET request returning a strongly typed object/list, which is automatically serialized to JSON.
- `ControllerRouteBuilder& MapGet(const String& subPath, String (TController::*action)(const String&))`: Maps a GET request taking a path parameter and returning a raw string.
- `template <typename TResult> ControllerRouteBuilder& MapGet(const String& subPath, TResult (TController::*action)(const String&))`: Maps a GET request taking a path parameter and returning a strongly typed object, which is automatically serialized to JSON.
- `template <typename TResource> ControllerRouteBuilder& MapPost(const String& subPath, String (TController::*action)(const TResource&))`: Maps a POST request taking an automatically deserialized request body, returning a raw string.
- `template <typename TResult, typename TResource> ControllerRouteBuilder& MapPost(const String& subPath, TResult (TController::*action)(const TResource&))`: Maps a POST request taking an automatically deserialized request body, returning a strongly typed object which is automatically serialized to JSON.
- `template <typename TResource> ControllerRouteBuilder& MapPut(const String& subPath, String (TController::*action)(const String&, const TResource&))`: Maps a PUT request taking a path parameter and an automatically deserialized request body, returning a raw string.
- `template <typename TResult, typename TResource> ControllerRouteBuilder& MapPut(const String& subPath, TResult (TController::*action)(const String&, const TResource&))`: Maps a PUT request taking a path parameter and an automatically deserialized request body, returning a strongly typed object which is automatically serialized to JSON.
- `ControllerRouteBuilder& MapDelete(const String& subPath, String (TController::*action)(const String&))`: Maps a DELETE request with a path parameter, returning a raw string.
- `template <typename TResult> ControllerRouteBuilder& MapDelete(const String& subPath, TResult (TController::*action)(const String&))`: Maps a DELETE request with a path parameter, returning a strongly typed object which is automatically serialized to JSON.

---

### Complete, Compile-Ready Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Func.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Http;

// 1. Define a dependency to inject
class IGreetingService : public virtual Object {
public:
    virtual ~IGreetingService() = default;
    virtual String Greet(const String& name) = 0;
};

class GreetingService : public IGreetingService {
public:
    String Greet(const String& name) override {
        return "Hello, " + name + "! Welcome to DotNetDupe WebServer.";
    }
};

int main() {
    // Create builder and register services
    auto builder = WebApplication::CreateBuilder();
    builder->GetServices().AddSingleton<IGreetingService, GreetingService>();

    auto app = builder->Build();

    // Configure minimal API route mappings
    app->MapGet("/", [](SmartPointer<HttpContext> context) {
        return String("DotNetDupe Minimal API Server");
    });

    app->MapGet("/greet", [app](SmartPointer<HttpContext> context) {
        // Retrieve query parameter 'name', default to 'Guest'
        String name = "Guest";
        context->GetRequest()->GetQuery().TryGetValue("name", name);

        // Resolve greeting service from DI
        auto greetingSvc = app->GetServices()->GetRequiredService<IGreetingService>();
        return greetingSvc->Greet(name);
    });

    // Run the server in a background thread to allow demonstration
    Thread serverThread([app]() {
        app->Run("http://127.0.0.1:19095");
    });
    serverThread.Start();
    Thread::Sleep(200); // Wait for the listener to bind

    // Client requests
    try {
        HttpClient client;
        
        Console::WriteLine("\n--- Client Request 1 (Home) ---");
        auto resp1 = client.Get("http://127.0.0.1:19095/");
        Console::Write("Response 1 Status: ");
        Console::WriteLine(resp1->GetStatusCode());
        Console::Write("Response 1 Body: ");
        Console::WriteLine(resp1->GetContent()->ReadAsString());

        Console::WriteLine("\n--- Client Request 2 (Greeting with query) ---");
        auto resp2 = client.Get("http://127.0.0.1:19095/greet?name=Alice");
        Console::Write("Response 2 Status: ");
        Console::WriteLine(resp2->GetStatusCode());
        Console::Write("Response 2 Body: ");
        Console::WriteLine(resp2->GetContent()->ReadAsString());

    } catch (const BasicException<char>& ex) {
        Console::Write("Client Exception: ");
        Console::WriteLine(ex.What());
    }

    // Stop server
    app->Stop();
    serverThread.Join();
    Console::WriteLine("\nServer stopped successfully.");

    return 0;
}
```
