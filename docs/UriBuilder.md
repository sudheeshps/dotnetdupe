### class `UriBuilder`

Provides a convenient way to modify the contents of a `Uri` instance.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/UriBuilder.h"

int main() {
    DotNetDupe::System::UriBuilder uriBuilder;
    uriBuilder.SetScheme(_T("http"));
    uriBuilder.SetHost(_T("www.google.com"));
    uriBuilder.SetPortValue(80);
    uriBuilder.SetPath(_T("search"));
    uriBuilder.SetQuery(_T("q=dotnet"));
    uriBuilder.SetFragment(_T("fragment"));
    std::wcout << uriBuilder.ToString().GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `UriBuilder()`

Initializes a new instance of the `UriBuilder` class.

##### `UriBuilder(const String& uri)`

Initializes a new instance of the `UriBuilder` class with the specified URI.

##### `UriBuilder(const Uri& uri)`

Initializes a new instance of the `UriBuilder` class with the specified `Uri` instance.

##### `String GetScheme() const`

Gets or sets the scheme name of the URI.

##### `void SetScheme(const String& scheme)`

Sets the scheme name of the URI.

##### `String GetHost() const`

Gets or sets the Domain Name System (DNS) host name or IP address of a server.

##### `void SetHost(const String& host)`

Sets the Domain Name System (DNS) host name or IP address of a server.

##### `int GetPort() const`

Gets or sets the port number of the URI.

##### `void SetPortValue(int port)`

Sets the port number of the URI.

##### `String GetPath() const`

Gets or sets the path to the resource referenced by the URI.

##### `void SetPath(const String& path)`

Sets the path to the resource referenced by the URI.

##### `String GetQuery() const`

Gets or sets any query information included in the URI.

##### `void SetQuery(const String& query)`

Sets any query information included in the URI.

##### `String GetFragment() const`

Gets or sets the fragment portion of the URI.

##### `void SetFragment(const String& fragment)`

Sets the fragment portion of the URI.

##### `String GetUserName() const`

Gets or sets the user name associated with the URI.

##### `void SetUserName(const String& userName)`

Sets the user name associated with the URI.

##### `String GetPassword() const`

Gets or sets the password associated with the URI.

##### `void SetPassword(const String& password)`

Sets the password associated with the URI.

##### `Uri GetUri() const`

Gets the `Uri` instance constructed by the `UriBuilder`.

##### `String ToString() const`

Returns the display string for the specified `UriBuilder` instance.
