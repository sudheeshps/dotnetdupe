### class `Uri`

Provides an object representation of a uniform resource identifier (URI) and easy access to the parts of the URI.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Uri.h"

int main() {
    DotNetDupe::System::Uri uri(_T("http://www.google.com:80/search?q=dotnet#fragment"));
    std::wcout << L"Scheme: " << uri.GetScheme().GetRawString() << std::endl;
    std::wcout << L"Host: " << uri.GetHost().GetRawString() << std::endl;
    std::wcout << L"Port: " << uri.GetPort() << std::endl;
    std::wcout << L"AbsolutePath: " << uri.GetAbsolutePath().GetRawString() << std::endl;
    std::wcout << L"Query: " << uri.GetQuery().GetRawString() << std::endl;
    std::wcout << L"Fragment: " << uri.GetFragment().GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `Uri(const String& uriString)`

Initializes a new instance of the `Uri` class with the specified URI.

##### `String GetAbsolutePath() const`

Gets the absolute path of the URI.

##### `String GetAbsoluteUri() const`

Gets the absolute URI.

##### `String GetAuthority() const`

Gets the authority component of the URI.

##### `String GetHost() const`

Gets the host component of this instance.

##### `int GetPort() const`

Gets the port number of this URI.

##### `String GetScheme() const`

Gets the scheme name for this URI.

##### `String GetQuery() const`

Gets any query information included in the specified URI.

##### `String GetFragment() const`

Gets the fragment portion of the URI.
