# UriBuilder

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/UriBuilder.h"`

Provides a custom constructor for uniform resource identifiers (URIs) and modifies URIs for the `Uri` class.

---

## Syntax

```cpp
class UriBuilder : public Object;
```

---

## Constructors

### `UriBuilder()`
Initializes a new instance of the `UriBuilder` class.

### `UriBuilder(const String& uri)` / `UriBuilder(const Uri& uri)`
Initializes a new instance of the `UriBuilder` class with the specified URI.

### `UriBuilder(const String& scheme, const String& host, int port = -1)`
Initializes a new instance of the `UriBuilder` class with the specified scheme, host, and port.

```cpp
UriBuilder builder("https", "api.example.com", 443);
```

---

## Properties & Mutators

- `String GetScheme() const` / `void SetScheme(const String& value)`
- `String GetHost() const` / `void SetHost(const String& value)`
- `int GetPort() const` / `void SetPort(int value)`
- `String GetPath() const` / `void SetPath(const String& value)`
- `String GetQuery() const` / `void SetQuery(const String& value)`
- `String GetFragment() const` / `void SetFragment(const String& value)`
- `String GetUserName() const` / `void SetUserName(const String& value)`
- `String GetPassword() const` / `void SetPassword(const String& value)`

### `Uri GetUri()`
Gets the `Uri` instance constructed by the `UriBuilder`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/UriBuilder.h"

using namespace DotNetDupe::System;

int main() {
    UriBuilder builder;
    builder.SetScheme("https");
    builder.SetHost("localhost");
    builder.SetPort(5001);
    builder.SetPath("api/v1/health");
    builder.SetQuery("check=full");

    Uri uri = builder.GetUri();
    Console::WriteLine("Built URI: {0}", uri.ToString());

    return 0;
}
```
