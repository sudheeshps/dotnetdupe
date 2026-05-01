### class `UriParser`

Parses a new URI scheme.

#### Methods

##### `static void Register(UriParser* uriParser, const String& schemeName, int defaultPort)`

Associates a `UriParser` with a scheme.

**Parameters:**
- `uriParser`: The parser to register.
- `schemeName`: The name of the scheme.
- `defaultPort`: The default port for the scheme.

**Usage:**
```cpp
GenericUriParser myParser;
UriParser::Register(&myParser, _T("myapp"), 1234);
```

##### `static bool IsKnownScheme(const String& schemeName)`

Indicates whether the parser for a scheme is registered.

**Parameters:**
- `schemeName`: The name of the scheme to check.

**Returns:**
- `true` if the scheme is known; otherwise, `false`.

**Usage:**
```cpp
bool known = UriParser::IsKnownScheme(_T("http"));
```

##### `static UriParser* GetParser(const String& schemeName)`

Gets the parser for a scheme.

**Parameters:**
- `schemeName`: The name of the scheme.

**Returns:**
- The `UriParser` for the specified scheme.

**Usage:**
```cpp
UriParser* parser = UriParser::GetParser(_T("http"));
```

##### `UriParser()`

Initializes a new instance of the `UriParser` class.

**Usage:**
```cpp
UriParser parser;
```

##### `virtual String GetComponents(const Uri& uri, UriComponents components, UriFormat format)`

Gets the components of a URI.

**Parameters:**
- `uri`: The `Uri` to parse.
- `components`: The components to retrieve.
- `format`: The format of the returned components.

**Returns:**
- A string that contains the components.

**Usage:**
```cpp
Uri myUri(_T("http://example.com/path"));
String host = parser.GetComponents(myUri, UriComponents::Host, UriFormat::UriEscaped);
```
