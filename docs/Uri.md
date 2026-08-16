# Uri

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Uri.h"`

Provides an object representation of a Uniform Resource Identifier (URI) and easy access to the parts of the URI (scheme, host, port, path, query, fragment).

---

## Syntax

```cpp
class Uri : public Object;
```

---

## Constructors

### `Uri(const String& uriString)`
Initializes a new instance of the `Uri` class with the specified URI string.

- **Throws:**
  - `FormatException`: If the URI string cannot be parsed.

```cpp
Uri uri("https://user:pass@api.example.com:8443/v1/users?page=1#profile");
```

---

## Static Methods

### `static String EscapeDataString(const String& stringToEscape)`
Converts a string to its escaped representation.

```cpp
String escaped = Uri::EscapeDataString("Hello World & Co."); // "Hello%20World%20%26%20Co."
```

### `static String UnescapeDataString(const String& stringToUnescape)`
Converts a string to its unescaped representation.

---

## Properties

### `String GetScheme() const`
Gets the scheme name for this URI (e.g. `"https"`, `"http"`, `"ftp"`).

### `String GetHost() const`
Gets the host component of this URI (e.g. `"api.example.com"`).

### `int GetPort() const`
Gets the port number of this URI (e.g. `8443`).

### `String GetAbsolutePath() const`
Gets the absolute path of the URI (e.g. `"/v1/users"`).

### `String GetQuery() const`
Gets any query information included in the specified URI (e.g. `"?page=1"`).

### `String GetFragment() const`
Gets the escaped URI fragment (e.g. `"#profile"`).

### `String GetUserInfo() const`
Gets the user name, password, or other user-specific information associated with the specified URI.

### `bool IsLoopback() const`
Gets a value indicating whether the specified `Uri` references the local host (`localhost`, `127.0.0.1`, `::1`).

---

## Example

```cpp
#include "System/Console.h"
#include "System/Uri.h"

using namespace DotNetDupe::System;

int main() {
    Uri endpoint("https://api.github.com:443/repos/dotnet/runtime?branch=main");

    Console::WriteLine("Scheme: {0}", endpoint.GetScheme());
    Console::WriteLine("Host:   {0}", endpoint.GetHost());
    Console::WriteLine("Port:   {0}", endpoint.GetPort());
    Console::WriteLine("Path:   {0}", endpoint.GetAbsolutePath());
    Console::WriteLine("Query:  {0}", endpoint.GetQuery());

    return 0;
}
```
