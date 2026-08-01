#include <filesystem>
#include "pch.h"
#include "gtest/gtest.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/RestClient.h"
#include "System/Threading/Thread.h"
#include "System/Convert.h"
#include "System/BasicException.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/HttpStatusCode.h"
#include "WebAppCore/Controllers/ControllerBase.h"
#include "WebAppCore/Controllers/ControllerRouteBuilder.h"
#include "WebAppCore/Server/WebAppServer.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Console.h"
#if defined(_WIN32)
#include <windows.h>
#endif
#include "System/Text/Json/JsonSerializer.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Controllers;
using namespace DotNetDupe::System::Text::Json;

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

    struct TestProduct {
        String Name;
        int Price = 0;
    };

    class ProductsController : public ControllerBase {
    public:
        ProductsController() = default;
        ~ProductsController() override = default;

        Collections::Generic::List<TestProduct> GetProducts() {
            Collections::Generic::List<TestProduct> list;
            list.Add(TestProduct{"Laptop", 1200});
            list.Add(TestProduct{"Phone", 800});
            return list;
        }

        String GetProductById(const String& id) {
            if (id == "1") {
                return Ok(TestProduct{"Laptop", 1200});
            }
            return NotFound("Product not found");
        }

        String CreateProduct(const TestProduct& product) {
            return Created(String("Created product: ") + product.Name + " for " + Convert::ToString(product.Price));
        }

        String UpdateProduct(const String& id, const TestProduct& product) {
            return Ok(String("Updated product ") + id + " to " + product.Name);
        }

        String DeleteProduct(const String& id) {
            if (id == "1") {
                return NoContent();
            }
            return NotFound("Cannot delete unknown product");
        }
    };

    struct AuthResponse {
        String Message;
    };

    class SecureController : public ControllerBase {
    public:
        SecureController() = default;
        ~SecureController() override = default;

        String GetSecretData() {
            Collections::Generic::Dictionary<String, String> claims;
            if (!Authorize("secure-key-123", claims)) {
                return "";
            }
            return Ok(AuthResponse{String("SecretData:") + claims["sub"]});
        }

        String GetAdminData() {
            Collections::Generic::Dictionary<String, String> claims;
            if (!Authorize("secure-key-123", "role", "admin", claims)) {
                return "";
            }
            return Ok(AuthResponse{String("AdminData:") + claims["sub"]});
        }
    };
}

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<WebApplicationTests::TestProduct> {
                    static JsonElement Write(const WebApplicationTests::TestProduct& value) {
                        JsonElement obj(JsonValueKind::Object);
                        JsonElement nameVal(value.Name);
                        JsonElement priceVal(static_cast<double>(value.Price));
                        obj.SetProperty("name", nameVal);
                        obj.SetProperty("price", priceVal);
                        return obj;
                    }

                    static WebApplicationTests::TestProduct Read(const JsonElement& element) {
                        WebApplicationTests::TestProduct p;
                        JsonElement prop;
                        if (element.TryGetProperty("name", prop)) p.Name = prop.GetString();
                        if (element.TryGetProperty("price", prop)) p.Price = prop.GetInt32();
                        return p;
                    }
                };

                template <>
                struct JsonConverter<WebApplicationTests::AuthResponse> {
                    static JsonElement Write(const WebApplicationTests::AuthResponse& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("message", JsonElement(value.Message));
                        return obj;
                    }

                    static WebApplicationTests::AuthResponse Read(const JsonElement& element) {
                        WebApplicationTests::AuthResponse r;
                        JsonElement prop;
                        if (element.TryGetProperty("message", prop)) r.Message = prop.GetString();
                        return r;
                    }
                };
            }
        }
    }
}

namespace WebApplicationTests {

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
                context->GetResponse()->SetStatusCode(HttpStatusCode::Created);
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
            ASSERT_EQ(resp1->GetStatusCode(), HttpStatusCode::OK);
            ASSERT_EQ(resp1->GetContent()->ReadAsString(), "Hello World");

            ASSERT_FALSE(resp2.IsNull());
            ASSERT_EQ(resp2->GetStatusCode(), HttpStatusCode::OK);
            ASSERT_EQ(resp2->GetContent()->ReadAsString(), "Hello from DI Service!");

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebApplication_WhenRouterExtended_MatchesPutDeleteAndPathParameters) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            auto app = builder->Build();

            // Register GET route with dynamic parameter
            app->MapGet("/api/users/{id}", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                String id;
                bool hasId = context->GetRequest()->GetRouteValues().TryGetValue("id", id);
                if (hasId) {
                    return String("User ID: ") + id;
                }
                return String("No ID");
            });

            // Register PUT route with dynamic parameter
            app->MapPut("/api/users/{id}", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                String id;
                bool hasId = context->GetRequest()->GetRouteValues().TryGetValue("id", id);
                String body = context->GetRequest()->GetBody();
                context->GetResponse()->SetStatusCode(HttpStatusCode::OK);
                if (hasId) {
                    return String("Updated User ") + id + " with data: " + body;
                }
                return String("Update Failed");
            });

            // Register DELETE route with dynamic parameter
            app->MapDelete("/api/users/{id}", [](SmartPointer<DotNetDupe::WebAppCore::Http::HttpContext> context) {
                String id;
                bool hasId = context->GetRequest()->GetRouteValues().TryGetValue("id", id);
                if (hasId) {
                    context->GetResponse()->SetStatusCode(HttpStatusCode::NoContent);
                    return String("");
                }
                context->GetResponse()->SetStatusCode(HttpStatusCode::BadRequest);
                return String("Delete Failed");
            });

            // Run the server in a background thread on a high port
            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28081");
            });
            serverThread.Start();
            
            // Register RAII guard
            ServerScopeGuard guard(app, serverThread);
            
            Thread::Sleep(200); // Allow server to start

            // When
            HttpClient client;
            
            // 1. Test GET with dynamic path parameter
            auto respGet = client.Get("http://127.0.0.1:28081/api/users/456");
            
            // 2. Test PUT with dynamic path parameter and body
            auto content = SmartPointer<StringContent>::NewShared("Alice");
            auto respPut = client.Put("http://127.0.0.1:28081/api/users/789", content);

            // 3. Test DELETE with dynamic path parameter
            auto respDelete = client.Delete("http://127.0.0.1:28081/api/users/999");

            // Then
            ASSERT_FALSE(respGet.IsNull());
            ASSERT_EQ((int)respGet->GetStatusCode(), 200);
            ASSERT_EQ(respGet->GetContent()->ReadAsString(), "User ID: 456");

            ASSERT_FALSE(respPut.IsNull());
            ASSERT_EQ((int)respPut->GetStatusCode(), 200);
            ASSERT_EQ(respPut->GetContent()->ReadAsString(), "Updated User 789 with data: Alice");

            ASSERT_FALSE(respDelete.IsNull());
            ASSERT_EQ((int)respDelete->GetStatusCode(), 204);
            ASSERT_TRUE(respDelete->GetContent()->ReadAsString().IsEmpty());

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebApplication_WhenControllerRouted_InvokesActionsAndSerializesCorrectly) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            // Register Controller and configure routes using the fluent builder API
            builder->AddController<ProductsController>("/api/products")
                .MapGet("", &ProductsController::GetProducts)
                .MapGet("/{id}", &ProductsController::GetProductById)
                .MapPost("", &ProductsController::CreateProduct)
                .MapPut("/{id}", &ProductsController::UpdateProduct)
                .MapDelete("/{id}", &ProductsController::DeleteProduct);

            auto app = builder->Build();

            // Maps controllers automatically under the hood, hiding the plumbing code
            app->MapControllers();

            // Run the server in a background thread on a high port
            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28082");
            });
            serverThread.Start();
            
            // Register RAII guard
            ServerScopeGuard guard(app, serverThread);
            
            Thread::Sleep(200); // Allow server to start

            // When
            HttpClient client;
            
            // 1. Test GET all
            auto respGetAll = client.Get("http://127.0.0.1:28082/api/products");
            
            // 2. Test GET by ID (Product 1 exists)
            auto respGet1 = client.Get("http://127.0.0.1:28082/api/products/1");

            // 3. Test GET by ID (Product 2 does not exist -> 404)
            auto respGet2 = client.Get("http://127.0.0.1:28082/api/products/2");

            // 4. Test POST product (JSON body binding)
            auto postContent = SmartPointer<StringContent>::NewShared("{\"name\":\"Tablet\",\"price\":500}");
            auto respPost = client.Post("http://127.0.0.1:28082/api/products", postContent);

            // 5. Test PUT product (path + JSON body binding)
            auto putContent = SmartPointer<StringContent>::NewShared("{\"name\":\"Keyboard\",\"price\":100}");
            auto respPut = client.Put("http://127.0.0.1:28082/api/products/10", putContent);

            // 6. Test DELETE product (Product 1 delete -> 204)
            auto respDelete1 = client.Delete("http://127.0.0.1:28082/api/products/1");

            // 7. Test DELETE product (Product 2 delete -> 404)
            auto respDelete2 = client.Delete("http://127.0.0.1:28082/api/products/2");

            // Then
            ASSERT_FALSE(respGetAll.IsNull());
            ASSERT_EQ((int)respGetAll->GetStatusCode(), 200);
            ASSERT_TRUE(respGetAll->GetContent()->ReadAsString().Contains("Laptop"));
            ASSERT_TRUE(respGetAll->GetContent()->ReadAsString().Contains("Phone"));

            ASSERT_FALSE(respGet1.IsNull());
            ASSERT_EQ((int)respGet1->GetStatusCode(), 200);
            ASSERT_TRUE(respGet1->GetContent()->ReadAsString().Contains("Laptop"));

            ASSERT_FALSE(respGet2.IsNull());
            ASSERT_EQ((int)respGet2->GetStatusCode(), 404);
            ASSERT_TRUE(respGet2->GetContent()->ReadAsString().Contains("Product not found"));

            ASSERT_FALSE(respPost.IsNull());
            ASSERT_EQ((int)respPost->GetStatusCode(), 201);
            ASSERT_TRUE(respPost->GetContent()->ReadAsString().Contains("Created product: Tablet for 500"));

            ASSERT_FALSE(respPut.IsNull());
            ASSERT_EQ((int)respPut->GetStatusCode(), 200);
            ASSERT_TRUE(respPut->GetContent()->ReadAsString().Contains("Updated product 10 to Keyboard"));

            ASSERT_FALSE(respDelete1.IsNull());
            ASSERT_EQ((int)respDelete1->GetStatusCode(), 204);

            ASSERT_FALSE(respDelete2.IsNull());
            ASSERT_EQ((int)respDelete2->GetStatusCode(), 404);

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebApplication_WhenRestClientUsed_PerformsTypedCrudOperations) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            builder->AddController<ProductsController>("/api/products")
                .MapGet("", &ProductsController::GetProducts)
                .MapGet("/{id}", &ProductsController::GetProductById)
                .MapPost("", &ProductsController::CreateProduct)
                .MapPut("/{id}", &ProductsController::UpdateProduct)
                .MapDelete("/{id}", &ProductsController::DeleteProduct);

            auto app = builder->Build();
            app->MapControllers();

            // Run the server in a background thread on a high port
            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28083");
            });
            serverThread.Start();
            
            // Register RAII guard
            ServerScopeGuard guard(app, serverThread);
            
            Thread::Sleep(200); // Allow server to start

            // When
            RestClient<TestProduct> client("http://127.0.0.1:28083/api/products");

            // 1. GetAll
            auto list = client.GetAll();
            
            // 2. Get by ID
            auto prod1 = client.Get("1");

            // 3. Post (Create)
            TestProduct newProd{"Tablet", 500};
            String postResult = client.Post(newProd);

            // 4. Put (Update)
            TestProduct updatedProd{"Keyboard", 100};
            String putResult = client.Put("10", updatedProd);

            // 5. Delete
            client.Delete("1");

            // Then
            ASSERT_EQ(list.GetCount(), 2);
            ASSERT_EQ(list[0].Name, "Laptop");
            ASSERT_EQ(list[1].Name, "Phone");

            ASSERT_EQ(prod1.Name, "Laptop");
            ASSERT_EQ(prod1.Price, 1200);

            ASSERT_TRUE(postResult.Contains("Created product: Tablet for 500"));
            ASSERT_TRUE(putResult.Contains("Updated product 10 to Keyboard"));

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebApplication_WhenAuthUsed_ValidatesJwtTokensCorrectly) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            builder->AddController<SecureController>("/api/secure")
                .MapGet("/secret", &SecureController::GetSecretData)
                .MapGet("/admin", &SecureController::GetAdminData);

            auto app = builder->Build();
            app->MapControllers();

            // Run the server in a background thread
            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28084");
            });
            serverThread.Start();
            
            // Register RAII guard
            ServerScopeGuard guard(app, serverThread);
            
            Thread::Sleep(200); // Allow server to start

            // Generate valid token for normal user
            DotNetDupe::System::IdentityModel::Tokens::Jwt::JWTToken userToken;
            userToken.GetPayload().Add("sub", "john_doe");
            userToken.GetPayload().Add("role", "user");
            String userTokenStr = userToken.CreateToken("secure-key-123");

            // Generate valid token for admin
            DotNetDupe::System::IdentityModel::Tokens::Jwt::JWTToken adminToken;
            adminToken.GetPayload().Add("sub", "admin_user");
            adminToken.GetPayload().Add("role", "admin");
            String adminTokenStr = adminToken.CreateToken("secure-key-123");

            // 1. Unauthenticated Request (Expect 401)
            RestClient<AuthResponse> client("http://127.0.0.1:28084/api/secure");
            try {
                client.Get("secret");
                FAIL() << "Should have thrown HttpRequestException for 401";
            } catch (const HttpRequestException&) {
                // Success
            }

            // 2. Authenticated Request as normal user (Expect 200)
            client.SetBearerToken(userTokenStr);
            auto secretRes = client.Get("secret");
            ASSERT_EQ(secretRes.Message, "SecretData:john_doe");

            // 3. Request admin endpoint as normal user (Expect 403)
            try {
                client.Get("admin");
                FAIL() << "Should have thrown HttpRequestException for 403";
            } catch (const HttpRequestException&) {
                // Success
            }

            // 4. Request admin endpoint as admin user (Expect 200)
            client.SetBearerToken(adminTokenStr);
            auto adminRes = client.Get("admin");
            ASSERT_EQ(adminRes.Message, "AdminData:admin_user");

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenController_WhenQueryParameterProvided_ExtractsQueryParameter) {
        try {
            // Given
            auto builder = WebApplication::CreateBuilder();
            builder->AddController<ProductsController>("/api/test")
                .MapGet("/item", &ProductsController::GetProductById);

            auto app = builder->Build();
            app->MapControllers();

            Thread serverThread([app]() {
                app->Run("http://127.0.0.1:28085");
            });
            serverThread.Start();
            
            ServerScopeGuard guard(app, serverThread);
            Thread::Sleep(200);

            // When
            RestClient<TestProduct> client("http://127.0.0.1:28085/api/test");
            auto product = client.Get("item?id=1");

            // Then
            ASSERT_EQ(product.Name, "Laptop");

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(WebApplicationTests, GivenWebAppServer_WhenStaticFileRequested_ServesHtmlAndAssets) {
        try {
            // Given: Create temporary wwwroot test directory & index.html
            String testWebRoot = DotNetDupe::System::IO::Path::GetFullPath("test_wwwroot");
            std::filesystem::create_directory(testWebRoot.GetRawString());
            String htmlPath = DotNetDupe::System::IO::Path::Combine({testWebRoot, "index.html"});
            String cssPath = DotNetDupe::System::IO::Path::Combine({testWebRoot, "site.css"});

            DotNetDupe::System::IO::File::WriteAllText(htmlPath, "<html><body>Hello WebServer</body></html>");
            DotNetDupe::System::IO::File::WriteAllText(cssPath, "body { color: red; }");

            auto builder = WebApplication::CreateBuilder();
            auto app = builder->Build();

            DotNetDupe::System::SmartPointer<DotNetDupe::WebAppCore::Server::WebAppServer> server = 
                DotNetDupe::System::SmartPointer<DotNetDupe::WebAppCore::Server::WebAppServer>::NewShared(app, testWebRoot);
            server->EnableStaticFiles("index.html");

            Thread serverThread([server]() {
                server->Run("http://127.0.0.1:28086");
            });
            serverThread.Start();

            ServerScopeGuard guard(app, serverThread);
            Thread::Sleep(500);

            HttpClient client;

            // 1. Request root '/' -> should serve index.html
            String htmlRes = client.GetString("http://127.0.0.1:28086/");
            ASSERT_EQ(htmlRes, "<html><body>Hello WebServer</body></html>");

            // 2. Request '/site.css' -> should serve CSS
            String cssRes = client.GetString("http://127.0.0.1:28086/site.css");
            ASSERT_EQ(cssRes, "body { color: red; }");

            // Clean up test files
            DotNetDupe::System::IO::File::Delete(htmlPath);
            DotNetDupe::System::IO::File::Delete(cssPath);

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }
}
