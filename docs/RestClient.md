# RestClient&lt;TResource&gt;

**Namespace:** `DotNetDupe::System::Net::Http`  
**Header:** `#include "System/Net/Http/RestClient.h"`

A strongly-typed REST client that encapsulates `HttpClient` and automatically serializes and deserializes C++ structures to and from JSON payloads using `JsonSerializer`.

---

## Syntax

```cpp
template <typename TResource>
class RestClient : public virtual Object;
```

---

## Constructors

### `RestClient(const String& sBaseUrl)`
Initializes a new instance of the `RestClient` configured for the specified base endpoint URL.

---

## Authentication & Headers

### `void SetBearerToken(const String& sToken)`
Configures the HTTP `Authorization` header with a Bearer token (`"Bearer <sToken>"`).

### `void SetBasicAuthentication(const String& sUsername, const String& sPassword)`
Configures the HTTP `Authorization` header with Base64 encoded credentials (`"Basic <base64>"`).

### `void ClearAuthentication()`
Removes authorization headers from subsequent requests.

### `void AddDefaultRequestHeader(const String& sName, const String& sValue)`
Adds or replaces a custom default header across all HTTP requests.

### `void RemoveDefaultRequestHeader(const String& sName)`
Removes the specified default header.

---

## CRUD Operations

### `List<TResource> GetAll()`
Sends `GET /api/resource` and deserializes the JSON array response into a `List<TResource>`.

### `TResource Get(const String& sId)`
Sends `GET /api/resource/{id}` and deserializes the JSON object response into `TResource`.

### `String Post(const TResource& resource)`
Serializes `resource` to JSON and sends `POST /api/resource`. Returns the raw response string.

### `template <typename TResult = TResource> TResult PostAndReturn(const TResource& resource)`
Serializes `resource`, sends `POST /api/resource`, and deserializes the JSON body into `TResult`.

### `String Put(const String& sId, const TResource& resource)`
Serializes `resource` to JSON and sends `PUT /api/resource/{id}`.

### `void Delete(const String& sId)`
Sends `DELETE /api/resource/{id}`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Net/Http/RestClient.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;

struct TodoItem {
    int id;
    int userId;
    String title;
    bool completed;
};

// Register JSON reflection
JSON_SERIALIZABLE(TodoItem, id, userId, title, completed)

int main() {
    RestClient<TodoItem> client("https://jsonplaceholder.typicode.com/todos");
    client.SetBearerToken("secret_jwt_token");

    // GET single item
    TodoItem item = client.Get("1");
    Console::WriteLine("Fetched Todo #{0}: {1} (Completed: {2})", 
        item.id, item.title, item.completed);

    // POST new item
    TodoItem newItem { 0, 1, "Ship DotNetDupe v4.0", false };
    TodoItem created = client.PostAndReturn(newItem);
    Console::WriteLine("Created Todo #{0}: {1}", created.id, created.title);

    return 0;
}
```
