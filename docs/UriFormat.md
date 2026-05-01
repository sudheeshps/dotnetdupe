### enum `UriFormat`

Controls how URI information is escaped.

#### Values

| Value | Description |
| :--- | :--- |
| `UriEscaped` | The URI is escaped. |
| `Unescaped` | The URI is unescaped. |
| `SafeUnescaped` | The URI is safely unescaped. |

**Usage:**
```cpp
UriFormat format = UriFormat::SafeUnescaped;
```
