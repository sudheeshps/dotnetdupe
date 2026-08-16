# Char

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Char.h"`

Represents a Unicode character code point (`char32_t`) and provides comprehensive static methods for inspecting, categorizing, and converting character values.

---

## Syntax

```cpp
class Char : public Object;
```

---

## Constructors & Assignment

### `Char()`
Initializes a new `Char` instance with code point `0` (null character).

### `Char(char32_t c)`
Initializes a new `Char` instance with the specified UTF-32 code point.

### `void operator=(char32_t c)`
Assigns a new character code point.

### `bool Equals(char32_t c) const` / `bool operator==(char32_t c) const`
Determines equality with a character code point.

### `char32_t GetChar() const`
Retrieves the underlying `char32_t` Unicode code point.

---

## Static Classification Methods

### General Unicode Categories
- `static bool IsDigit(char32_t c)`: Indicates whether the character is a decimal digit (`0-9`).
- `static bool IsLetter(char32_t c)`: Indicates whether the character is a Unicode letter.
- `static bool IsLetterOrDigit(char32_t c)`: Indicates whether the character is a letter or decimal digit.
- `static bool IsUpper(char32_t c)`: Indicates whether the character is uppercase.
- `static bool IsLower(char32_t c)`: Indicates whether the character is lowercase.
- `static bool IsNumber(char32_t c)`: Indicates whether the character is categorized as a number.
- `static bool IsPunctuation(char32_t c)`: Indicates whether the character is punctuation.
- `static bool IsWhiteSpace(char32_t c)`: Indicates whether the character is white space.
- `static bool IsControl(char32_t c)`: Indicates whether the character is a control character.
- `static bool IsSeparator(char32_t c)`: Indicates whether the character is a separator character.
- `static bool IsSurrogate(char32_t c)`: Indicates whether the character is a surrogate code point.
- `static bool IsSymbol(char32_t c)`: Indicates whether the character is categorized as a symbol.

### ASCII Specific Methods
- `static bool IsAscii(char32_t c)`: Indicates whether the character is in the ASCII range ($0 \le c \le 127$).
- `static bool IsAsciiDigit(char32_t c)`: Indicates whether the character is an ASCII decimal digit (`0-9`).
- `static bool IsAsciiLetter(char32_t c)`: Indicates whether the character is an ASCII letter (`a-z`, `A-Z`).
- `static bool IsAsciiLetterLower(char32_t c)`: Indicates whether the character is an ASCII lowercase letter (`a-z`).
- `static bool IsAsciiLetterUpper(char32_t c)`: Indicates whether the character is an ASCII uppercase letter (`A-Z`).
- `static bool IsAsciiLetterOrDigit(char32_t c)`: Indicates whether the character is an ASCII letter or digit.
- `static bool IsAsciiHexDigit(char32_t c)`: Indicates whether the character is a hexadecimal digit (`0-9`, `a-f`, `A-F`).
- `static bool IsAsciiHexDigitLower(char32_t c)`: Indicates whether the character is a lowercase hex digit (`0-9`, `a-f`).
- `static bool IsAsciiHexDigitUpper(char32_t c)`: Indicates whether the character is an uppercase hex digit (`0-9`, `A-F`).

### Range Checking
- `static bool IsBetween(char32_t c, char32_t minInclusive, char32_t maxInclusive)`: Indicates whether the character falls inclusively within the specified range.

---

## Static Conversion Methods

### `static char32_t ToLower(char32_t c)`
Converts the character to its lowercase equivalent.

### `static char32_t ToUpper(char32_t c)`
Converts the character to its uppercase equivalent.

### `static double GetNumericValue(char32_t c)`
Converts the specified numeric Unicode character to a double-precision floating-point number (e.g. returns `5.0` for `'5'`).

### `String ToString() const` / `static String ToString(char32_t c)`
Converts the Unicode code point into a single-character UTF-8 encoded `String`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Char.h"

using namespace DotNetDupe::System;

int main() {
    char32_t ch = 'A';

    Console::WriteLine("Is ASCII Letter:     {0}", Char::IsAsciiLetter(ch));
    Console::WriteLine("Is Hex Digit:        {0}", Char::IsAsciiHexDigit(ch));
    Console::WriteLine("Is Lower:            {0}", Char::IsLower(ch));
    Console::WriteLine("Converted Lowercase: {0}", Char::ToString(Char::ToLower(ch)));
    Console::WriteLine("Numeric Value of '9':{0}", Char::GetNumericValue('9'));

    return 0;
}
```
