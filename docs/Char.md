### template class `BasicChar<CharT>`

Represents a character.

**Note:** The `Char` class is a typedef for `BasicChar<TCHAR>`.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Char.h"

int main() {
    DotNetDupe::System::Char c = 'a';
    std::wcout << L"Is 'a' a letter? " << (DotNetDupe::System::Char::IsLetter(c.GetChar()) ? L"Yes" : L"No") << std::endl;
    std::wcout << L"Is 'a' a digit? " << (DotNetDupe::System::Char::IsDigit(c.GetChar()) ? L"Yes" : L"No") << std::endl;

    return 0;
}
```

#### Methods

##### `BasicChar(CharT c)`

Initializes a new instance of the `BasicChar` class with the specified character.

##### `void operator= (CharT c)`

Assigns a character to the `BasicChar` object.

##### `bool Equals(CharT c)`

Indicates whether this instance and a specified `CharT` object are equal.

##### `bool operator == (CharT c)`

Compares this instance to a specified `CharT` object.

##### `double static GetNumericValue(CharT c)`

Converts the specified numeric Unicode character to a double-precision floating-point number.

##### `bool static IsAscii(CharT c)`

Indicates whether the specified character is an ASCII character.

##### `bool static IsAsciiDigit(CharT c)`

Indicates whether the specified character is an ASCII digit.

##### `bool static IsAsciiHexDigit(CharT c)`

Indicates whether the specified character is an ASCII hexadecimal digit.

##### `bool static IsAsciiHexDigitLower(CharT c)`

Indicates whether the specified character is a lowercase ASCII hexadecimal digit.

##### `bool static IsAsciiHexDigitUpper(CharT c)`

Indicates whether the specified character is an uppercase ASCII hexadecimal digit.

##### `bool static IsAsciiLetter(CharT c)`

Indicates whether the specified character is an ASCII letter.

##### `bool static IsAsciiLetterLower(CharT c)`

Indicates whether the specified character is a lowercase ASCII letter.

##### `bool static IsAsciiLetterOrDigit(CharT c)`

Indicates whether the specified character is an ASCII letter or digit.

##### `bool static IsAsciiLetterUpper(CharT c)`

Indicates whether the specified character is an uppercase ASCII letter.

##### `bool static IsBetween(CharT c, CharT minInclusive, CharT maxInclusive)`

Indicates whether the specified character is between the specified minimum and maximum inclusive values.

##### `bool static IsControl(CharT c)`

Indicates whether the specified character is a control character.

##### `bool static IsDigit(CharT c)`

Indicates whether the specified character is a decimal digit.

##### `bool static IsLower(CharT c)`

Indicates whether the specified character is a lowercase letter.

##### `bool static IsNumber(CharT c)`

Indicates whether the specified character is a number.

##### `bool static IsLetter(CharT c)`

Indicates whether the specified character is a letter.

##### `bool static IsLetterOrDigit(CharT c)`

Indicates whether the specified character is a letter or a decimal digit.

##### `CharT static ToLower(CharT c)`

Converts the value of a specified Unicode character to its lowercase equivalent.

##### `CharT GetChar()`

Returns the character value of this instance.
