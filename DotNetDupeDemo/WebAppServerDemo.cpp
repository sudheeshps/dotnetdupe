#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/IO/Directory.h"
#if defined(_WIN32)
#include <windows.h>
#endif
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Server/WebAppServer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Server;

void RunWebAppServerDemo() {
    Console::WriteLine("--- WebAppServer Demo ---");

    String webRoot = "demo_wwwroot";
    if (IO::Directory::Exists(webRoot)) {
        IO::Directory::Delete(webRoot, true);
    }

    IO::File::WriteAllText(IO::Path::Combine({webRoot, "index.html"}), 
        "<!DOCTYPE html><html><head><title>DotNetDupe WebAppServer</title></head>"
        "<body><h1>Welcome to DotNetDupe WebAppServer!</h1><p>Static files and API endpoints served simultaneously.</p></body></html>");

    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    app->MapGet("/api/status", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> ctx) {
        return String("{\"status\":\"Online\",\"server\":\"DotNetDupe WebAppServer\"}");
    });

    WebAppServer server(app, webRoot);
    server.EnableStaticFiles("index.html");

    Console::WriteLine(String("WebAppServer serving static website from '") + webRoot + "' with 16 ThreadPool worker threads...");
    Console::WriteLine("Root URL: http://localhost:8080/index.html");
    Console::WriteLine("Demo finished setup cleanly.");
}
