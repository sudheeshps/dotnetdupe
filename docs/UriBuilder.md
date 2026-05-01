### class `UriBuilder`

Provides a convenient way to modify the contents of a `Uri` instance.

#### Methods

##### `UriBuilder()`

Initializes a new instance of the `UriBuilder` class.

**Usage:**
```cpp
UriBuilder builder;
```

##### `UriBuilder(const String& uri)`

Initializes a new instance of the `UriBuilder` class with the specified URI string.

**Parameters:**
- `uri`: A URI string.

**Usage:**
```cpp
UriBuilder builder(_T("http://example.com"));
```

##### `UriBuilder(const Uri& uri)`

Initializes a new instance of the `UriBuilder` class with the specified `Uri` instance.

**Parameters:**
- `uri`: An instance of the `Uri` class.

**Usage:**
```cpp
Uri myUri(_T("http://example.com"));
UriBuilder builder(myUri);
```

##### `String GetScheme() const`

Gets the scheme name of the URI.

**Returns:**
- The scheme of the URI.

**Usage:**
```cpp
String scheme = builder.GetScheme();
```

##### `void SetScheme(const String& scheme)`

Sets the scheme name of the URI.

**Parameters:**
- `scheme`: The scheme name.

**Usage:**
```cpp
builder.SetScheme(_T("https"));
```

##### `String GetHost() const`

Gets the Domain Name System (DNS) host name or IP address of a server.

**Returns:**
- The host name or IP address of the URI.

**Usage:**
```cpp
String host = builder.GetHost();
```

##### `void SetHost(const String& host)`

Sets the Domain Name System (DNS) host name or IP address of a server.

**Parameters:**
- `host`: The host name or IP address.

**Usage:**
```cpp
builder.SetHost(_T("www.example.com"));
```

##### `int GetPort() const`

Gets the port number of the URI.

**Returns:**
- The port number of the URI.

**Usage:**
```cpp
int port = builder.GetPort();
```

##### `void SetPortValue(int port)`

Sets the port number of the URI.

**Parameters:**
- `port`: The port number.

**Usage:**
```cpp
builder.SetPortValue(8080);
```

##### `String GetPath() const`

Gets the path to the resource referenced by the URI.

**Returns:**
- The path to the resource.

**Usage:**
```cpp
String path = builder.GetPath();
```

##### `void SetPath(const String& path)`

Sets the path to the resource referenced by the URI.

**Parameters:**
- `path`: The path to the resource.

**Usage:**
```cpp
builder.SetPath(_T("/api/v1/data"));
```

##### `String GetQuery() const`

Gets any query information included in the URI.

**Returns:**
- The query information included in the URI.

**Usage:**
```cpp
String query = builder.GetQuery();
```

##### `void SetQuery(const String& query)`

Sets any query information included in the URI.

**Parameters:**
- `query`: The query information.

**Usage:**
```cpp
builder.SetQuery(_T("id=123"));
```

##### `String GetFragment() const`

Gets the fragment portion of the URI.

**Returns:**
- The fragment portion of the URI.

**Usage:**
```cpp
String fragment = builder.GetFragment();
```

##### `void SetFragment(const String& fragment)`

Sets the fragment portion of the URI.

**Parameters:**
- `fragment`: The fragment portion.

**Usage:**
```cpp
builder.SetFragment(_T("section1"));
```

##### `String GetUserName() const`

Gets the user name associated with the URI.

**Returns:**
- The user name associated with the URI.

**Usage:**
```cpp
String user = builder.GetUserName();
```

##### `void SetUserName(const String& userName)`

Sets the user name associated with the URI.

**Parameters:**
- `userName`: The user name.

**Usage:**
```cpp
builder.SetUserName(_T("admin"));
```

##### `String GetPassword() const`

Gets the password associated with the URI.

**Returns:**
- The password associated with the URI.

**Usage:**
```cpp
String password = builder.GetPassword();
```

##### `void SetPassword(const String& password)`

Sets the password associated with the URI.

**Parameters:**
- `password`: The password.

**Usage:**
```cpp
builder.SetPassword(_T("secret"));
```

##### `Uri GetUri() const`

Gets the `Uri` instance constructed by the `UriBuilder`.

**Returns:**
- A `Uri` containing the URI constructed by the `UriBuilder`.

**Usage:**
```cpp
Uri finalUri = builder.GetUri();
```

##### `String ToString() const`

Returns the display string for the specified `UriBuilder` instance.

**Returns:**
- The string that contains the unescaped display string of the `UriBuilder`.

**Usage:**
```cpp
String uriStr = builder.ToString();
```
