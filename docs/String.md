# String

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/String.h"`

Represents text as a sequence of UTF-8 encoded code units. Provides rich, C#-like string operations including formatting, splitting with `StringSplitOptions`, joining, index lookups (`IndexOf`, `LastIndexOf`, `IndexOfAny`), substring extraction, trimming, and comparisons with ABI stability.

---

## Syntax

```cpp
enum class StringSplitOptions {
    None,
    RemoveEmptyEntries,
    TrimEntries
};

class String;
```

---

## Constructors & Assignment

### `String()`
Initializes a new empty `String` instance.

### `String(const char* pStr)`
Initializes a `String` from a null-terminated UTF-8 C-string.
- **Throws:**
  - `ArgumentException`: If `pStr` is `nullptr`.

### `String(const wchar_t* pWStr)`
Converting constructor that automatically transcodes from a UTF-16 wide character string into UTF-8.

### `String(const String& sStr)` / `String(String&& sStr) noexcept`
Copy and move constructors.

### `String& operator=(const String& sStr)` / `operator=(const char* pStr)` / `operator=(const wchar_t* pStr)`
Assignment operators.

---

## Properties & Basic Operations

### `int GetLength() const`
Gets the number of characters in the current `String` object.

### `bool IsEmpty() const`
Indicates whether the string is empty (`GetLength() == 0`).

### `const char* GetRawString() const`
Retrieves a pointer to the null-terminated UTF-8 byte buffer.

### `char operator[](int iIndex) const`
Gets the character at a specified character position.
- **Throws:**
  - `ArgumentOutOfRangeException`: If `iIndex < 0` or `iIndex >= GetLength()`.

### `int GetHashCode() const`
Calculates an integer hash code for this string instance.

### `String Clone() const`
Creates a copy of this `String` instance.

---

## Comparison & Equality

### `static int Compare(const String& sStr1, int iIndex1, const String& sStr2, int iIndex2, int iLength, bool bIgnoreCase)`
Compares substrings of two specified `String` objects and returns an integer that indicates their relative position in the sort order.

### `int CompareTo(const String& sStr) const`
Compares this instance with a specified `String` object and indicates whether this instance precedes, follows, or appears in the same position in the sort order.

### `static bool Equals(const String& sStr1, const String& sStr2)` / `bool Equals(const String& sStr) const`
Determines whether two string instances have the same value.

### `bool operator==(const String& other) const` / `!=` / `<`
Comparison operators for string equality and ordering.

---

## Searching & Indexing

### `bool Contains(char ch) const` / `bool Contains(const String& sStr) const`
Returns a value indicating whether a specified character or substring occurs within this string.

### `int IndexOf(const String& sSubstring, int iStartIndex = 0, bool bIgnoreCase = false) const`
Reports the zero-based index of the first occurrence of the specified string in this instance. Returns `-1` if not found.

### `int IndexOfAny(int iStartIndex, std::initializer_list<char> chChars)`
Reports the zero-based index of the first occurrence in this instance of any character in a specified character list.

### `int LastIndexOf(const String& sStr, bool bIgnoreCase = false)`
Reports the zero-based index position of the last occurrence of a specified string within this instance.

### `int LastIndexOfAny(int iStartIndex, std::initializer_list<char> chChars, bool bIgnoreCase = false)`
Reports the zero-based index position of the last occurrence in this instance of one or more specified characters.

### `bool StartsWith(const String& sPrefix, bool bIgnoreCase = false) const`
Determines whether the beginning of this string instance matches the specified string.

### `bool EndsWith(char ch, bool bIgnoreCase = false) const` / `EndsWith(const String& sSuffix, bool bIgnoreCase = false) const`
Determines whether the end of this string instance matches the specified string or character.

---

## Formatting, Splitting & Joining

### `template <typename... Args> static String Format(const char* pFormat, const Args&... args)` / `Format(const String& sFormat, ...)`
Replaces each format item in a composite format string with the text equivalent of the corresponding argument value.

```cpp
String sMsg = String::Format("User {0} logged in from {1} at port {2}", "Alice", "127.0.0.1", 8080);
```

### `Array<String> Split(char chSeparator) const`
Splits a string into substrings based on a specified delimiter character.

### `Array<String> Split(const Array<String>& arrSeparators, StringSplitOptions eOptions = StringSplitOptions::None) const`
Splits a string into substrings based on specified delimiter strings and options (`RemoveEmptyEntries`, `TrimEntries`).

### `Array<String> Split(const String* pSeparator, int iCount, StringSplitOptions eOptions) const`
Splits a string into substrings based on an array of delimiter strings.

### `static String Join(char chSeparator, std::initializer_list<String> sStrings, int iStartIndex = 0, int iCount = -1)`
Concatenates all elements of a string array using the specified character delimiter.

### `static String Join(const String& sSeparator, std::initializer_list<String> sStrings, int iStartIndex = 0, int iCount = -1)`
Concatenates all elements of a string array using the specified string delimiter.

---

## String Manipulation & Mutation

### `String& Append(char ch)` / `String& Append(const String& sStr)`
Appends a character or string to this instance and returns a reference.

### `String& Insert(int iIndex, const String& sStr)`
Inserts a specified string at a specified index position in this instance.

### `String Remove(int iStartIndex) const` / `String Remove(int iStartIndex, int iCount) const`
Returns a new string in which a specified number of characters from the current string have been deleted.

### `String Replace(char chOriginalChar, char chReplaceChar) const`
Returns a new string in which all occurrences of a specified character are replaced.

### `String Replace(const String& sOriginalStr, const String& sReplaceStr) const`
Returns a new string in which all occurrences of a specified substring in the current instance are replaced with another specified string.

### `String Substring(int iStartIndex) const` / `String Substring(int iStartIndex, int iLength) const`
Retrieves a substring from this instance.
- **Throws:**
  - `ArgumentOutOfRangeException`: If index or length is outside string bounds.

### `String ToLower() const` / `String ToUpper() const`
Returns a copy of this string converted to lowercase or uppercase.

### `String Trim() const` / `String TrimStart() const` / `String TrimEnd() const`
Removes all leading and trailing white-space characters.

### `String PadLeft(int iTotalWidth, char ch = ' ')` / `String PadRight(int iTotalWidth, char ch = ' ')`
Right-aligns or left-aligns the characters in this instance by padding them with characters on the left or right.

### `void CopyTo(int iSourceIndex, char* pDestination, int iDestinationIndex, int iDestArraySize, int iCount) const`
Copies a specified number of characters from a specified position in this instance to a specified position in an array of characters.

---

## Static Conversions

- `static String FromInt(int iValue)`
- `static String ValueOf(int iValue)` / `ValueOf(long long llValue)` / `ValueOf(double dValue)`
- `static String ToString(int iValue)` / `ToString(long long llValue)` / `ToString(double dValue)` / `ToString(bool bValue)`

---

## Example

```cpp
#include "System/Console.h"
#include "System/String.h"
#include "System/Array.h"

using namespace DotNetDupe::System;

int main() {
    String sText = "  Framework, DotNetDupe, C++  ";
    String sTrimmed = sText.Trim();

    Array<String> separators(1);
    separators[0] = ",";

    Array<String> parts = sTrimmed.Split(separators, StringSplitOptions::TrimEntries);
    for (int i = 0; i < parts.GetLength(); ++i) {
        Console::WriteLine("Part [{0}]: '{1}'", i, parts[i]);
    }

    int lastComma = sTrimmed.LastIndexOf(",");
    Console::WriteLine("Last comma index: {0}", lastComma);

    return 0;
}
```
