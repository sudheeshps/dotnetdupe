### template class `BasicString<CharT>`

Represents text as a sequence of UTF-16 code units.

**Note:** The `String` class is a typedef for `BasicString<TCHAR>`.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/String.h"

int main() {
    DotNetDupe::System::String s1 = _T("Hello");
    DotNetDupe::System::String s2 = _T("World");
    DotNetDupe::System::String s3 = s1 + _T(" ") + s2;
    std::wcout << s3.GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `BasicString(const CharT* pStr)`

Initializes a new instance of the `BasicString` class with a pointer to a null-terminated character array.

##### `const CharT* GetRawString() const`

Returns a pointer to the raw character array.

##### `int GetLength() const`

Returns the length of the string.

##### `std::basic_string<CharT>& GetString()`

Returns a reference to the underlying `std::basic_string` object.

##### `BasicString<CharT> Clone() const`

Creates a new `BasicString` object with the same value as this instance.

##### `bool operator<(const BasicString<CharT>& str) const`

Compares this instance with another `BasicString` object.

##### `bool operator==(const BasicString<CharT>& str) const`

Compares this instance with another `BasicString` object for equality.

##### `bool operator!=(const BasicString<CharT>& str) const`

Compares this instance with another `BasicString` object for inequality.

##### `CharT operator[](int index) const`

Gets the character at the specified index.

##### `int static Compare(const BasicString<CharT>& str1, int index1, const BasicString<CharT>& str2, int index2, int length, bool ignoreCase)`

Compares two specified `BasicString` objects.

##### `int CompareTo(const BasicString& str) const`

Compares this instance with a specified `BasicString` object.

##### `BasicString<CharT> operator+(const BasicString<CharT>& str) const`

Concatenates two `BasicString` objects.

##### `BasicString<CharT> Concat(const std::initializer_list<BasicString<CharT>> strs) const`

Concatenates the members of a collection of `BasicString` objects.

##### `bool Contains(CharT c) const`

Determines whether a character occurs within this string.

##### `bool Contains(const BasicString<CharT>& str)`

Determines whether a specified substring occurs within this string.

##### `void CopyTo(int sourceIndex, CharT* pDestination, int destinationIndex, int destArraySize, int count) const`

Copies a specified number of characters from a specified position in this instance to a specified position in a character array.

##### `bool EndsWith(CharT c, bool ignoreCase) const`

Determines whether the end of this string instance matches the specified character.

##### `bool EndsWith(const BasicString<CharT>& suffix, bool ignoreCase) const`

Determines whether the end of this string instance matches the specified string.

##### `bool static Equals(const BasicString<CharT>& str1, const BasicString<CharT>& str2)`

Determines whether two specified `BasicString` objects have the same value.

##### `bool Equals(const BasicString<CharT>& str) const`

Determines whether this instance and a specified `BasicString` object have the same value.

##### `int IndexOf(const BasicString<CharT>& substring) const`

Returns the zero-based index of the first occurrence of a specified substring in this instance.

##### `int IndexOf(const BasicString<CharT>& substring, bool ignoreCase) const`

Returns the zero-based index of the first occurrence of a specified substring in this instance, using a specified comparison option.

##### `int IndexOf(const BasicString<CharT>& substring, int startIndex, bool ignoreCase) const`

Returns the zero-based index of the first occurrence of a specified substring in this instance, starting the search at a specified character position and using a specified comparison option.

##### `int IndexOfAny(int startIndex, std::initializer_list<CharT> chars)`

Returns the zero-based index of the first occurrence in this instance of any character in a specified array of Unicode characters.

##### `BasicString<CharT>& Append(const CharT ch)`

Appends a character to this instance.

##### `BasicString<CharT>& Append(const BasicString<CharT>& str)`

Appends a `BasicString` object to this instance.

##### `BasicString<CharT>& Insert(int index, const BasicString<CharT>& str)`

Inserts a specified string at a specified index position in this instance.

##### `bool IsEmpty() const`

Indicates whether this string is empty.

##### `BasicString<CharT> static Join(CharT separator, std::initializer_list<BasicString<CharT>> strings)`

Concatenates the elements of a collection, using the specified separator between each element.

##### `BasicString<CharT> static Join(CharT separator, std::initializer_list<BasicString<CharT>> strings, int startIndex, int count)`

Concatenates the elements of a collection, using the specified separator between each element, starting from a specified index and concatenating a specified number of elements.

##### `BasicString<CharT> static Join(const BasicString<CharT>& separator, std::initializer_list<BasicString<CharT>> strings)`

Concatenates the elements of a collection, using the specified separator string between each element.

##### `BasicString<CharT> static Join(const BasicString<CharT>& separator, std::initializer_list<BasicString<CharT>> strings, int startIndex, int count)`

Concatenates the elements of a collection, using the specified separator string between each element, starting from a specified index and concatenating a specified number of elements.

##### `int LastIndexOf(const BasicString<CharT>& str, bool ignoreCase)`

Returns the zero-based index of the last occurrence of a specified string in this instance.

##### `int LastIndexOfAny(int startIndex, std::initializer_list<CharT> chars, bool ignoreCase)`

Returns the zero-based index of the last occurrence in this instance of one or more characters specified in a Unicode character array.

##### `BasicString<CharT> PadLeft(int totalWidth)`

Returns a new string that right-aligns the characters in this instance by padding them on the left with spaces, for a specified total length.

##### `BasicString<CharT> PadLeft(int totalWidth, CharT ch)`

Returns a new string that right-aligns the characters in this instance by padding them on the left with a specified character, for a specified total length.

##### `BasicString<CharT> PadRight(int totalWidth)`

Returns a new string that left-aligns the characters in this instance by padding them on the right with spaces, for a specified total length.

##### `BasicString<CharT> PadRight(int totalWidth, CharT ch)`

Returns a new string that left-aligns the characters in this instance by padding them on the right with a specified character, for a specified total length.

##### `BasicString<CharT> Remove(int startIndex) const`

Deletes all the characters from this instance beginning at a specified position to the end of the string.

##### `BasicString<CharT> Remove(int startIndex, int count) const`

Deletes a specified number of characters from this instance beginning at a specified position.

##### `BasicString<CharT> Replace(CharT originalChar, CharT replaceChar)`

Returns a new string in which all occurrences of a specified Unicode character in this instance are replaced with another specified Unicode character.

##### `BasicString<CharT> Replace(const BasicString<CharT>& orignalStr, const BasicString<CharT>& replaceStr)`

Returns a new string in which all occurrences of a specified string in the current instance are replaced with another specified string.

##### `std::vector<BasicString<CharT>> Split(CharT separator)`

Splits a string into substrings that are based on the characters in an array.

##### `std::vector<BasicString<CharT>> Split(BasicString<CharT> separator [], int count, StringSplitOptions options)`

Splits a string into substrings that are based on the characters in an array.

##### `bool StartsWith(const BasicString<CharT>& prefix, bool ignoreCase) const`

Determines whether the beginning of this string instance matches the specified string.

##### `BasicString<CharT> Substring(int startIndex, int length) const`

Retrieves a substring from this instance.
