### class `Uri`

Provides an object representation of a uniform resource identifier (URI) and easy access to the parts of the URI.

#### Methods

##### `Uri(const String& uriString)`

Initializes a new instance of the `Uri` class with the specified URI.

**Parameters:**
- `uriString`: A string that represents the URI.

**Usage:**
```cpp
Uri uri("http://example.com/path?query=1");
```

##### `String GetAbsolutePath() const`

Gets the absolute path of the URI.

**Returns:**
- A string containing the absolute path of the URI.

**Usage:**
```cpp
String path = uri.GetAbsolutePath();
```

##### `String GetAbsoluteUri() const`

Gets the absolute URI.

**Returns:**
- A string containing the entire URI.

**Usage:**
```cpp
String fullUri = uri.GetAbsoluteUri();
```

##### `String GetAuthority() const`

Gets the authority component of the URI.

**Returns:**
- A string containing the authority component of the URI.

**Usage:**
```cpp
String authority = uri.GetAuthority();
```

##### `String GetHost() const`

Gets the host component of this instance.

**Returns:**
- A string containing the host name.

**Usage:**
```cpp
String host = uri.GetHost();
```

##### `int GetPort() const`

Gets the port number of this URI.

**Returns:**
- An integer containing the port number of the URI.

**Usage:**
```cpp
int port = uri.GetPort();
```

##### `String GetScheme() const`

Gets the scheme name for this URI.

**Returns:**
- A string containing the scheme (e.g., "http" or "https").

**Usage:**
```cpp
String scheme = uri.GetScheme();
```

##### `String GetQuery() const`

Gets any query information included in the specified URI.

**Returns:**
- A string containing the query information.

**Usage:**
```cpp
String query = uri.GetQuery();
```

##### `String GetFragment() const`

Gets the fragment portion of the URI.

**Returns:**
- A string containing the fragment information.

**Usage:**
```cpp
String fragment = uri.GetFragment();
```

---

## Code Example

The following example demonstrates how to parse a URI, read its various components, and manage the `Uri` instance dynamically using `SmartPointer`. It also shows how to catch a custom `FormatException` if one occurs.

```cpp
#include "System/Console.h"
#include "System/Uri.h"
#include "System/SmartPointer.h"
#include "System/FormatException.h"

using namespace DotNetDupe::System;

int main() {
    try {
        // Construct Uri using SmartPointer
        auto pUri = SmartPointer<Uri>::New("https://admin:secret@example.com:8080/path/to/resource?query=1#section");

        Console::Write("Scheme: ");
        Console::WriteLine(pUri->GetScheme());

        Console::Write("Host: ");
        Console::WriteLine(pUri->GetHost());

        Console::Write("Port: ");
        Console::WriteLine(pUri->GetPort());

        Console::Write("Absolute Path: ");
        Console::WriteLine(pUri->GetAbsolutePath());

        Console::Write("Query: ");
        Console::WriteLine(pUri->GetQuery());

        Console::Write("Fragment: ");
        Console::WriteLine(pUri->GetFragment());

        Console::Write("UserInfo: ");
        Console::WriteLine(pUri->GetUserInfo());

        Console::Write("Is Loopback: ");
        Console::WriteLine(pUri->IsLoopback());

    } catch (const FormatException& ex) {
        Console::Write("Error parsing URI: ");
        Console::WriteLine(ex.What());
    }

    return 0;
}
```


