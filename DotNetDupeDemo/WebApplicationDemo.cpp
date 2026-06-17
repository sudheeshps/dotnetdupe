#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Func.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Threading/Thread.h"
#include "Demos.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Http;

namespace {
    class IInfoService : public virtual Object {
    public:
        virtual ~IInfoService() = default;
        virtual String GetInfo() = 0;
    };

    class InfoService : public IInfoService {
    public:
        String GetInfo() override {
            return "DotNetDupe Web Host v1.0";
        }
    };
}

void DemonstrateWebApplication() {
    Console::WriteLine("\n=== Web Application Demonstration ===");

    // 1. Create Builder & Register Service
    auto builder = WebApplication::CreateBuilder();
    builder->GetServices().AddSingleton<IInfoService, InfoService>();

    // 2. Build the App
    auto app = builder->Build();

    // 3. Define endpoints (Minimal APIs)
    app->MapGet("/", [](SmartPointer<HttpContext> context) {
        return String("Welcome to the WebApplication Minimal API Endpoint!");
    });

    app->MapGet("/info", [app](SmartPointer<HttpContext> context) {
        auto infoSvc = app->GetServices()->GetRequiredService<IInfoService>();
        return infoSvc->GetInfo();
    });

    // Dynamic Route Parameter demonstration
    app->MapGet("/api/users/{id}", [](SmartPointer<HttpContext> context) {
        String id;
        if (context->GetRequest()->GetRouteValues().TryGetValue("id", id)) {
            return String("Fetched User Profile for ID: ") + id;
        }
        return String("User ID not provided");
    });

    app->MapPut("/api/users/{id}", [](SmartPointer<HttpContext> context) {
        String id;
        if (context->GetRequest()->GetRouteValues().TryGetValue("id", id)) {
            String body = context->GetRequest()->GetBody();
            context->GetResponse()->SetStatusCode(200);
            return String("Successfully updated user ") + id + " with content: " + body;
        }
        context->GetResponse()->SetStatusCode(400);
        return String("Bad Request");
    });

    app->MapDelete("/api/users/{id}", [](SmartPointer<HttpContext> context) {
        String id;
        if (context->GetRequest()->GetRouteValues().TryGetValue("id", id)) {
            context->GetResponse()->SetStatusCode(204); // No Content
            return String("");
        }
        context->GetResponse()->SetStatusCode(400);
        return String("Bad Request");
    });

    // 4. Start the server asynchronously in a background Thread
    Console::WriteLine("Starting WebApplication on http://127.0.0.1:19099...");
    Thread serverThread([app]() {
        app->Run("http://127.0.0.1:19099");
    });
    serverThread.Start();

    // Give the server thread a moment to start and bind
    Thread::Sleep(200);

    // 5. Use HttpClient to send requests to our WebApplication
    try {
        HttpClient client;

        Console::WriteLine("\n[Client] Sending GET request to '/'...");
        auto resp1 = client.Get("http://127.0.0.1:19099/");
        Console::Write("[Client] Response Status: ");
        Console::WriteLine((int)resp1->GetStatusCode());
        Console::Write("[Client] Response Body: '");
        Console::Write(resp1->GetContent()->ReadAsString());
        Console::WriteLine("'");

        Console::WriteLine("\n[Client] Sending GET request to '/info'...");
        auto resp2 = client.Get("http://127.0.0.1:19099/info");
        Console::Write("[Client] Response Status: ");
        Console::WriteLine((int)resp2->GetStatusCode());
        Console::Write("[Client] Response Body: '");
        Console::Write(resp2->GetContent()->ReadAsString());
        Console::WriteLine("'");

        Console::WriteLine("\n[Client] Sending GET request to '/api/users/12345' (Dynamic Path Parameter)...");
        auto resp3 = client.Get("http://127.0.0.1:19099/api/users/12345");
        Console::Write("[Client] Response Status: ");
        Console::WriteLine((int)resp3->GetStatusCode());
        Console::Write("[Client] Response Body: '");
        Console::Write(resp3->GetContent()->ReadAsString());
        Console::WriteLine("'");

        Console::WriteLine("\n[Client] Sending PUT request to '/api/users/12345' (Update)...");
        auto content = SmartPointer<StringContent>::NewShared("{\"name\": \"Alice\"}");
        auto resp4 = client.Put("http://127.0.0.1:19099/api/users/12345", content);
        Console::Write("[Client] Response Status: ");
        Console::WriteLine((int)resp4->GetStatusCode());
        Console::Write("[Client] Response Body: '");
        Console::Write(resp4->GetContent()->ReadAsString());
        Console::WriteLine("'");

        Console::WriteLine("\n[Client] Sending DELETE request to '/api/users/12345'...");
        auto resp5 = client.Delete("http://127.0.0.1:19099/api/users/12345");
        Console::Write("[Client] Response Status: ");
        Console::WriteLine((int)resp5->GetStatusCode());
        Console::Write("[Client] Response Body: '");
        Console::Write(resp5->GetContent()->ReadAsString());
        Console::WriteLine("' (Expected empty for 204)");

    } catch (const BasicException<char>& ex) {
        Console::Write("[Client Exception] Error: ");
        Console::WriteLine(ex.What());
    }

    // 6. Shut down the server cleanly
    Console::WriteLine("\nStopping WebApplication...");
    app->Stop();
    serverThread.Join();
    Console::WriteLine("WebApplication stopped.");
    Console::WriteLine("=====================================");
}
