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

### `UriBuilder(const String& uri)`
Initializes a new instance of the `UriBuilder` class with the specified URI string.

### `UriBuilder(const Uri& uri)`
Initializes a new instance of the `UriBuilder` class with the specified `Uri` instance.

### `UriBuilder(const String& scheme, const String& host)`
Initializes a new instance of the `UriBuilder` class with the specified scheme and host.

### `UriBuilder(const String& scheme, const String& host, int port)`
Initializes a new instance of the `UriBuilder` class with the specified scheme, host, and port number.

```cpp
UriBuilder objBuilder("https", "api.example.com", 443);
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

### `String ToString()`
Gets the string representation of the constructed URI.

---

## Example

```cpp
#include "System/Console.h"
#include "System/UriBuilder.h"

using namespace DotNetDupe::System;

int main() {
    UriBuilder objBuilder;
    objBuilder.SetScheme("https");
    objBuilder.SetHost("localhost");
    objBuilder.SetPort(5001);
    objBuilder.SetPath("api/v1/health");
    objBuilder.SetQuery("check=full");

    Uri objUri = objBuilder.GetUri();
    Console::WriteLine("Built URI: {0}", objUri.ToString());

    return 0;
}
```
