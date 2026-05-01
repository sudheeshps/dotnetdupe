### enum `UriComponents`

Specifies the parts of a URI.

#### Values

| Value | Description |
| :--- | :--- |
| `Scheme` | The scheme name. |
| `UserInfo` | The user name and password. |
| `Host` | The host name. |
| `Port` | The port number. |
| `Path` | The path. |
| `Query` | The query information. |
| `Fragment` | The fragment. |
| `StrongPort` | The port number, if it is not the default port for the scheme. |
| `NormalizedHost` | The normalized host name. |
| `KeepDelimiter` | A delimiter is kept. |
| `AbsoluteUri` | The absolute URI. |
| `PathAndQuery` | The path and query. |
| `SchemeAndServer` | The scheme and server. |
| `HttpRequestUrl` | The HTTP request URL. |
| `UriPath` | The URI path. |
| `HostAndPort` | The host and port. |
| `UserInfoAndHostAndPort` | The user info, host, and port. |
| `SafeUnescaped` | The unescaped URI. |

**Usage:**
```cpp
UriComponents components = UriComponents::HostAndPort;
```
