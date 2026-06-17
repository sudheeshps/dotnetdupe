#include "pch.h"
#include "gtest/gtest.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Threading/Thread.h"
#include "System/BasicException.h"
#include "System/Net/Http/HttpRequestException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;

namespace WebApplicationTests {

    class ITestService : public virtual Object {
    public:
        virtual ~ITestService() = default;
        virtual String GetMessage() = 0;
    };

    class TestService : public ITestService {
    public:
        String GetMessage() override {
            return "Hello from DI Service!";
        }
    };

    // RAII guard to guarantee thread join and server stop on test exit/failure
    struct ServerScopeGuard {
        SmartPointer<WebApplication> App;
        Thread& ServerThread;
        ServerScopeGuard(SmartPointer<WebApplication> app, Thread& t) : App(app), ServerThread(t) {}
        ~ServerScopeGuard() {
            if (!App.IsNull()) {
                App->Stop();
            }
            ServerThread.Join();
        }
    };

    TEST(WebApplicationTests, GivenBuilderCreated_WhenServicesConfigured_BuildsWebApplication) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            builder->GetServices().AddSingleton<ITestService, TestService>();

            // When
            auto app = builder->Build();

            // Then
            ASSERT_FALSE(app.IsNull());
            auto spService = app->GetServices()->GetService<ITestService>();
            ASSERT_FALSE(spService.IsNull());
            ASSERT_EQ(spService->GetMessage(), "Hello from DI Service!");
        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebApplication_WhenRoutesMapped_HandlesHttpRequests) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            builder->GetServices().AddSingleton<ITestService, TestService>();
            auto app = builder->Build();

            app->MapGet("/hello", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                auto svc = context->GetRequest(); // Just a dummy access to request
                return String("Hello World");
            });

            app->MapGet("/service", [app](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                auto spSvc = app->GetServices()->GetRequiredService<ITestService>();
                return spSvc->GetMessage();
            });

            app->MapPost("/echo", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                String body = context->GetRequest()->GetBody();
                context->GetResponse()->SetStatusCode(201);
                return body;
            });

            // Run the server in a background thread on a high port
            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28080");
            });
            serverThread.Start();
            
            // Register RAII guard
            ServerScopeGuard guard(app, serverThread);
            
            Thread::Sleep(200); // Allow server to start

            // When
            HttpClient client;
            SmartPointer<HttpResponseMessage> resp1;
            
            // Retry connection to handle slow server startup/port binding
            int retries = 5;
            while (retries > 0) {
                try {
                    resp1 = client.Get("http://127.0.0.1:28080/hello");
                    break;
                } catch (const BasicException<char>&) {
                    retries--;
                    if (retries == 0) throw;
                    Thread::Sleep(200);
                }
            }
            
            auto resp2 = client.Get("http://127.0.0.1:28080/service");

            // Then
            ASSERT_FALSE(resp1.IsNull());
            ASSERT_EQ((int)resp1->GetStatusCode(), 200);
            ASSERT_EQ(resp1->GetContent()->ReadAsString(), "Hello World");

            ASSERT_FALSE(resp2.IsNull());
            ASSERT_EQ((int)resp2->GetStatusCode(), 200);
            ASSERT_EQ(resp2->GetContent()->ReadAsString(), "Hello from DI Service!");

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }
}
