# ControllerBase

**Namespace:** `DotNetDupe::WebAppCore::Controllers`  
**Header:** `#include "WebAppCore/Controllers/ControllerBase.h"`

Base class for ASP.NET-style MVC and Web API REST controllers providing request context access, JSON action results (`Ok`, `Created`, `NotFound`, `BadRequest`), and JWT authorization helpers.

---

## Syntax

```cpp
class ControllerBase : public virtual DotNetDupe::System::Object;
```

---

## Protected Members & Context

- `SmartPointer<Http::HttpContext> m_httpContext`: Encapsulates current HTTP transaction.
- `SmartPointer<Http::HttpRequest> Request() const`: Access to incoming headers, query params, and body.
- `SmartPointer<Http::HttpResponse> Response() const`: Access to outgoing status code, headers, and body.

---

## Helper Methods

### `template <typename U> String Ok(const U& value)`
Sets HTTP status 200 OK, sets content type to `application/json`, and serializes `value` into JSON.

### `template <typename U> String Created(const U& value)`
Sets HTTP status 201 Created and serializes `value` into JSON.

### `String NoContent()`
Sets HTTP status 204 No Content.

### `String NotFound(const String& error = "Not Found")`
Sets HTTP status 404 Not Found with JSON error message.

### `String BadRequest(const String& error = "Bad Request")`
Sets HTTP status 400 Bad Request with JSON error message.

### `String Unauthorized(const String& error = "Unauthorized")`
Sets HTTP status 401 Unauthorized with JSON error message.

### `String Forbidden(const String& error = "Forbidden")`
Sets HTTP status 403 Forbidden with JSON error message.

### `bool Authorize(const String& sSecretKey, Dictionary<String, String>& claims)`
Validates the incoming HTTP `Authorization: Bearer <token>` against the provided HMAC secret key and populates extracted token claims.

---

## Example

```cpp
#include "WebAppCore/Controllers/ControllerBase.h"
#include "System/Collections/Generic/List.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Controllers;

struct UserDto {
    int Id;
    String Name;
};

class UsersController : public ControllerBase {
public:
    // GET /api/users
    String GetAllUsers() {
        Collections::Generic::List<UserDto> users;
        users.Add(UserDto{1, "Alice"});
        users.Add(UserDto{2, "Bob"});
        return Ok(users);
    }

    // GET /api/users/secure
    String GetSecureProfile() {
        Collections::Generic::Dictionary<String, String> claims;
        if (!Authorize("SuperSecretKey123", claims)) {
            return Unauthorized("Authentication failed");
        }
        return Ok(String("Welcome user: ") + claims["sub"]);
    }
};
```
