### Web API Authentication & Authorization Guide

DotNetDupe provides complete support for JWT Bearer token authentication and role/claim-based authorization, matching the developer-friendly patterns of ASP.NET Core and .NET Base Class Library.

This guide details how to configure and use authentication on both the client side (via `RestClient` or `HttpClient`) and the server side (via `ControllerBase`).

---

### Client-Side Authentication

The strongly-typed `RestClient<TResource>` provides helper methods to attach authentication credentials to default request headers. These headers are automatically sent with every outgoing request.

#### Methods

##### `void SetBearerToken(const String& token)`
Attaches a JWT Bearer token to the `Authorization` header. Format: `Authorization: Bearer <token>`.

##### `void SetBasicAuthentication(const String& username, const String& password)`
Encodes the provided username and password using Base64, and attaches it to the `Authorization` header. Format: `Authorization: Basic <base64-credentials>`.

##### `void ClearAuthentication()`
Removes the `Authorization` header from the default request headers.

##### `void AddDefaultRequestHeader(const String& name, const String& value)`
Adds a custom default request header.

##### `void RemoveDefaultRequestHeader(const String& name)`
Removes a custom default request header.

---

### Server-Side Authentication & Authorization

Controllers inheriting from `ControllerBase` can authenticate and authorize incoming requests against a secret key using JWTs.

#### Methods

##### `bool Authorize(const String& secretKey, Dictionary<String, String>& claims)`
*   Extracts the Bearer token from the incoming request's `Authorization` header.
*   Validates the signature using `secretKey`.
*   If valid, populates the `claims` dictionary with JWT payload claims and returns `true`.
*   If missing, invalid, or expired, sets the HTTP response status to `401 Unauthorized` with a JSON error payload and returns `false`.

##### `bool Authorize(const String& secretKey, const String& requiredClaim, const String& requiredValue, Dictionary<String, String>& claims)`
*   Performs signature verification first (as above).
*   Verifies if the specified `requiredClaim` (e.g., `"role"` or `"scope"`) exists and matches `requiredValue`.
*   If signature verification fails, returns `false` (with `401 Unauthorized`).
*   If the claim check fails, sets the response status code to `403 Forbidden` with a JSON error payload and returns `false`.

##### `String Forbidden(const String& error = "Forbidden")`
Returns a `403 Forbidden` status with a JSON error payload.

---

### Complete, Compile-Ready Example

This example demonstrates how to set up an authorized controller on the server, generate signed JWT tokens, and request secure resources using `RestClient`.

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Func.h"
#include "System/Collections/Generic/List.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/Net/Http/RestClient.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Controllers/ControllerBase.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Controllers;

// 1. Define the resource returned by the server
struct UserProfile {
    String Username;
    String Role;
};

// 2. Register JsonConverter for UserProfile
namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<UserProfile> {
                    static JsonElement Write(const UserProfile& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("username", JsonElement(value.Username));
                        obj.SetProperty("role", JsonElement(value.Role));
                        return obj;
                    }

                    static UserProfile Read(const JsonElement& element) {
                        UserProfile u;
                        JsonElement prop;
                        if (element.TryGetProperty("username", prop)) u.Username = prop.GetString();
                        if (element.TryGetProperty("role", prop)) u.Role = prop.GetString();
                        return u;
                    }
                };
            }
        }
    }
}

// 3. Define the secure controller
class SecureUserController : public ControllerBase {
public:
    SecureUserController() = default;
    ~SecureUserController() override = default;

    // Requires authentication
    String GetProfile() {
        Dictionary<String, String> claims;
        if (!Authorize("my-secret-key-12345", claims)) {
            return ""; // Response headers & body are set inside Authorize
        }
        return Ok(UserProfile{claims["sub"], claims["role"]});
    }

    // Requires 'admin' role
    String GetAdminConfig() {
        Dictionary<String, String> claims;
        if (!Authorize("my-secret-key-12345", "role", "admin", claims)) {
            return ""; // Response status (401/403) set inside Authorize
        }
        return Ok(String("System Config: Online"));
    }
};

int main() {
    try {
        // Start WebServer
        auto builder = WebApplication::CreateBuilder();
        builder->AddController<SecureUserController>("/api/user")
            .MapGet("/profile", &SecureUserController::GetProfile)
            .MapGet("/config", &SecureUserController::GetAdminConfig);

        auto app = builder->Build();
        app->MapControllers();

        Thread serverThread([app]() {
            app->Run("http://127.0.0.1:19900");
        });
        serverThread.Start();
        Thread::Sleep(200); // Allow server to bind

        // Generate user token
        JWTToken uToken;
        uToken.GetPayload().Add("sub", "john_user");
        uToken.GetPayload().Add("role", "member");
        String userTokenStr = uToken.CreateToken("my-secret-key-12345");

        // Generate admin token
        JWTToken aToken;
        aToken.GetPayload().Add("sub", "admin_bob");
        aToken.GetPayload().Add("role", "admin");
        String adminTokenStr = aToken.CreateToken("my-secret-key-12345");

        // Client operations
        RestClient<UserProfile> client("http://127.0.0.1:19900/api/user");

        // 1. Fetch profile without credentials (Expect 401 Unauthorized)
        try {
            Console::WriteLine("Fetching profile without token...");
            client.Get("profile");
        } catch (const HttpRequestException& ex) {
            Console::WriteLine("Request failed as expected: " + String(ex.What()));
        }

        // 2. Fetch profile with valid member credentials (Expect 200 OK)
        Console::WriteLine("\nAuthenticating client as john_user...");
        client.SetBearerToken(userTokenStr);
        UserProfile profile = client.Get("profile");
        Console::WriteLine("User Profile:");
        Console::WriteLine(" - Username: " + profile.Username);
        Console::WriteLine(" - Role:     " + profile.Role);

        // 3. Request admin endpoint as john_user (Expect 403 Forbidden)
        RestClient<String> adminClient("http://127.0.0.1:19900/api/user");
        adminClient.SetBearerToken(userTokenStr);
        try {
            Console::WriteLine("\nAccessing admin config as john_user...");
            adminClient.Get("config");
        } catch (const HttpRequestException& ex) {
            Console::WriteLine("Access denied: " + String(ex.What()));
        }

        // 4. Request admin endpoint as admin_bob (Expect 200 OK)
        Console::WriteLine("\nAccessing admin config as admin_bob...");
        adminClient.SetBearerToken(adminTokenStr);
        String config = adminClient.Get("config");
        Console::WriteLine("Result: " + config);

        // Cleanup
        app->Stop();
        serverThread.Join();

    } catch (const BasicException<char>& ex) {
        Console::WriteLine("Error: " + String(ex.What()));
    }

    return 0;
}
```
