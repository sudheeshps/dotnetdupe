# StringConvert

**Namespace:** `DotNetDupe::System::Utils`  
**Header:** `#include "System/Utils/StringConvert.h"`

Provides internal cross-platform helper methods for UTF-8 and UTF-16 character transcoding.

---

## Syntax

```cpp
class StringConvert;
```

---

## Static Methods

### `static std::string WCharToUtf8(const wchar_t* pWStr)`
Transcodes a wide character (UTF-16) string pointer to a standard UTF-8 string.

### `static std::wstring Utf8ToWChar(const char* pUtf8Str)`
Transcodes a UTF-8 character string pointer to a wide character (UTF-16) `std::wstring`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Utils/StringConvert.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Utils;

int main() {
    const wchar_t* wText = L"Wide UTF-16 String";
    std::string utf8 = StringConvert::WCharToUtf8(wText);

    Console::WriteLine("Converted to UTF-8: {0}", String(utf8.c_str()));
    return 0;
}
```
