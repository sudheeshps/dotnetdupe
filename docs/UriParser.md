### class `UriParser`

Parses a new URI scheme.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/UriParser.h"
#include "../DotNetDupe/Uri.h"

int main() {
    DotNetDupe::System::GenericUriParser genericParser;
    DotNetDupe::System::UriParser::Register(&genericParser, _T("custom"), 8080);

    if (DotNetDupe::System::UriParser::IsKnownScheme(_T("custom"))) {
        std::wcout << L"Custom scheme is known." << std::endl;
    }
    return 0;
}
```

#### Methods

##### `static void Register(UriParser* uriParser, const String& schemeName, int defaultPort)`

Associates a `UriParser` with a scheme.

##### `static bool IsKnownScheme(const String& schemeName)`

Indicates whether the parser for a scheme is registered.

##### `static UriParser* GetParser(const String& schemeName)`

Gets the parser for a scheme.

##### `UriParser()`

Initializes a new instance of the `UriParser` class.

##### `virtual String GetComponents(const Uri& uri, UriComponents components, UriFormat format)`

Gets the components of a URI.
