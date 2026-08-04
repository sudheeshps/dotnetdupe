# StringConvert Class Documentation

`DotNetDupe::System::Utils::StringConvert` provides static utility methods for converting strings between UTF-8 (`char` / `std::string`) and UTF-16 (`wchar_t` / `std::wstring`) encoding representations across Windows and Linux.

## Namespace
`DotNetDupe::System::Utils`

## Header
```cpp
#include "System/Utils/StringConvert.h"
```

## Public Static Methods

### `WCharToUtf8`
Converts a null-terminated UTF-16 wide character string (`const wchar_t*`) into a `DotNetDupe::System::String`.
```cpp
static String WCharToUtf8(const wchar_t* pWStr);
```

### `Utf8ToWChar`
Converts a null-terminated UTF-8 character string (`const char*`) into a `DotNetDupe::System::BasicString<wchar_t>`.
```cpp
static BasicString<wchar_t> Utf8ToWChar(const char* pUtf8Str);
```

## Example Usage

```cpp
#include "System/Utils/StringConvert.h"
#include "System/String.h"
#include <iostream>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Utils;

int main() {
    // 1. Convert UTF-16 to DotNetDupe String
    const wchar_t* pwszWide = L"Hello, World!";
    String sUtf8 = StringConvert::WCharToUtf8(pwszWide);
    std::cout << "UTF-8 string: " << sUtf8.GetRawString() << std::endl;

    // 2. Convert UTF-8 to BasicString<wchar_t>
    const char* pszNarrow = "DotNetDupe Text Conversion";
    BasicString<wchar_t> sWChar = StringConvert::Utf8ToWChar(pszNarrow);

    return 0;
}
```
