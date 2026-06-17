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
