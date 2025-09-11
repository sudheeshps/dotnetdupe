# API Documentation

## Namespace: `DotNetDupe::System::IO`

### class `Path`

A class for path-related operations.

#### Methods

```cpp
// Example usage of Path class
#include <iostream>
#include <vector>
#include "DotNetDupe/Path.h"
#include "DotNetDupe/String.h" // Assuming String is available

int main() {
    // ChangeExtension
    DotNetDupe::System::String filePath = _T("document.txt");
    DotNetDupe::System::String newPath = DotNetDupe::System::IO::Path::ChangeExtension(filePath, _T("pdf"));
    std::wcout << L"Changed extension: " << newPath.GetRawString() << std::endl;

    // Combine
    DotNetDupe::System::String combinedPath = DotNetDupe::System::IO::Path::Combine({_T("C:\\Users"), _T("Document.txt")});
    std::wcout << L"Combined path: " << combinedPath.GetRawString() << std::endl;

    // EndsInDirectorySeparator
    bool endsWithSeparator = DotNetDupe::System::IO::Path::EndsInDirectorySeparator(_T("C:\\MyFolder\\"));
    std::wcout << L"Ends with separator: " << (endsWithSeparator ? L"true" : L"false") << std::endl;

    // Exists (requires actual file/folder to exist)
    bool exists = DotNetDupe::System::IO::Path::Exists(_T("C:\\Windows"));
    std::wcout << L"C:\\Windows exists: " << (exists ? L"true" : L"false") << std::endl;

    // GetDirectoryName
    DotNetDupe::System::String dirName = DotNetDupe::System::IO::Path::GetDirectoryName(_T("C:\\MyFolder\\MyFile.txt"));
    std::wcout << L"Directory name: " << dirName.GetRawString() << std::endl;

    // GetFileName
    DotNetDupe::System::String fileName = DotNetDupe::System::IO::Path::GetFileName(_T("C:\\MyFolder\\MyFile.txt"));
    std::wcout << L"File name: " << fileName.GetRawString() << std::endl;

    // GetExtension
    DotNetDupe::System::String extension = DotNetDupe::System::IO::Path::GetExtension(_T("MyFile.txt"));
    std::wcout << L"Extension: " << extension.GetRawString() << std::endl;

    // GetFileNameWithoutExtension
    DotNetDupe::System::String fileNameWithoutExt = DotNetDupe::System::IO::Path::GetFileNameWithoutExtension(_T("MyFile.txt"));
    std::wcout << L"File name without extension: " << fileNameWithoutExt.GetRawString() << std::endl;

    // GetFullPath (depends on current working directory)
    DotNetDupe::System::String fullPath = DotNetDupe::System::IO::Path::GetFullPath(_T("MyFile.txt"));
    std::wcout << L"Full path: " << fullPath.GetRawString() << std::endl;

    // GetInvalidFileNameChars
    std::vector<TCHAR> invalidFileChars = DotNetDupe::System::IO::Path::GetInvalidFileNameChars();
    std::wcout << L"Invalid file name chars count: " << invalidFileChars.size() << std::endl;

    // GetInvalidPathChars
    std::vector<TCHAR> invalidPathChars = DotNetDupe::System::IO::Path::GetInvalidPathChars();
    std::wcout << L"Invalid path chars count: " << invalidPathChars.size() << std::endl;

    return 0;
}
```

#### Methods


##### `static String ChangeExtension(const String &filePath, const String &extension)`

Changes the extension of a path string.

##### `static String Combine(const std::initializer_list<String> paths)`

Combines multiple strings into a path.

##### `static bool EndsInDirectorySeparator(const String &filePath)`

Determines whether the path ends in a directory separator.

##### `static bool Exists(const String &filePath)`

Determines whether the given path refers to an existing file or directory.

##### `static String GetDirectoryName(const String &filePath)`

Returns the directory information for the specified path.

##### `static String GetFileName(const String &filePath)`

Returns the file name and extension of the specified path string.

##### `static String GetExtension(const String &filePath)`

Returns the extension of the specified path string.

##### `static String GetFileNameWithoutExtension(const String &filePath)`

Returns the file name of the specified path string without the extension.

##### `static String GetFullPath(const String &path)`

Returns the absolute path for the specified path string.

##### `static std::vector<TCHAR> GetInvalidFileNameChars()`

Gets an array of characters that are not allowed in file names.

##### `static std::vector<TCHAR> GetInvalidPathChars()`

Gets an array of characters that are not allowed in path names.

## Namespace: `DotNetDupe::System`

### template class `BasicArgumentException<CharT>`

Inherits from `BasicException<CharT>`. Represents errors that occur during argument processing.

#### Methods

```cpp
// Example usage of BasicArgumentException
#include <iostream>
#include "DotNetDupe/BasicArgumentException.h"
#include "DotNetDupe/String.h" // Assuming String is available

void myFunction(int value) {
    if (value < 0) {
        throw DotNetDupe::System::BasicArgumentException<TCHAR>(_T("Value cannot be negative."));
    }
    // ...
}

int main() {
    try {
        myFunction(-5);
    } catch (const DotNetDupe::System::BasicArgumentException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }
    return 0;
}
```

##### `BasicArgumentException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### template class `BasicArgumentOutOfRangeException<CharT>`

Inherits from `BasicException<CharT>`. Represents errors that occur when an argument is outside the allowable range of values.

#### Methods

```cpp
// Example usage of BasicArgumentOutOfRangeException
#include <iostream>
#include "DotNetDupe/BasicArgumentOutOfRangeException.h"
#include "DotNetDupe/String.h" // Assuming String is available

void checkRange(int value) {
    if (value < 0 || value > 100) {
        throw DotNetDupe::System::BasicArgumentOutOfRangeException<TCHAR>(_T("Value must be between 0 and 100."));
    }
    // ...
}

int main() {
    try {
        checkRange(150);
    } catch (const DotNetDupe::System::BasicArgumentOutOfRangeException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }
    return 0;
}
```

##### `BasicArgumentOutOfRangeException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentOutOfRangeException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### template class `BasicArithmeticException<CharT>`

Inherits from `BasicSystemException<CharT>`. Represents errors in an arithmetic operation.

#### Methods

```cpp
// Example usage of BasicArithmeticException
#include <iostream>
#include "DotNetDupe/BasicArithmeticException.h"
#include "DotNetDupe/String.h" // Assuming String is available

int divide(int a, int b) {
    if (b == 0) {
        throw DotNetDupe::System::BasicArithmeticException<TCHAR>(_T("Division by zero."));
    }
    return a / b;
}

int main() {
    try {
        int result = divide(10, 0);
    } catch (const DotNetDupe::System::BasicArithmeticException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }
    return 0;
}
```

##### `BasicArithmeticException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArithmeticException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### template class `BasicChar<CharT>`

Represents a character.

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

## Namespace: `DotNetDupe::System`

### template class `BasicException<CharT>`

Represents errors that occur during application execution.

#### Methods

```cpp
// Example usage of BasicException class
#include <iostream>
#include "DotNetDupe/BasicException.h"

int main() {
    try {
        throw DotNetDupe::System::BasicException<TCHAR>(_T("Something went wrong!"));
    } catch (const DotNetDupe::System::BasicException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }
    return 0;
}
```

##### `BasicException(const CharT* pchMessage)`

Initializes a new instance of the `BasicException` class with a specified error message.

##### `CharT* What() const`

Returns a pointer to the error message.

## Namespace: `DotNetDupe::System`

### template class `BasicNotImplementedException<CharT>`

Inherits from `BasicException<CharT>`. The exception that is thrown when a requested method or operation is not implemented.

#### Methods

##### `BasicNotImplementedException(const CharT* pchMessage)`

Initializes a new instance of the `BasicNotImplementedException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### template class `BasicOverflowException<CharT>`

Inherits from `BasicArithmeticException<CharT>`. The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow.

#### Methods

##### `BasicOverflowException(const CharT* pchMessage)`

Initializes a new instance of the `BasicOverflowException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### enum `StringSplitOptions`

Specifies options for string splitting.

-   `None`: No options specified.
-   `RemoveEmptyEntries`: The return value does not include array elements that contain an empty string.
-   `TrimEntries`: Trims leading and trailing white-space characters from the array elements.

### template class `BasicString<CharT>`

Represents text as a sequence of UTF-16 code units.

#### Methods

##### `BasicString(const CharT* pStr)`

Initializes a new instance of the `BasicString` class with a pointer to a null-terminated character array.

##### `BasicString(const BasicString<CharT>& str)`

Copy constructor. Initializes a new instance of the `BasicString` class from an existing `BasicString` object.

##### `BasicString<CharT>& operator=(const BasicString<CharT>& str)`

Copy assignment operator. Assigns the value of an existing `BasicString` object to this instance.

##### `BasicString<CharT>(const BasicString<CharT>&& str) noexcept`

Move constructor. Initializes a new instance of the `BasicString` class by moving the resources from an existing `BasicString` object.

##### `BasicString<CharT>& operator=(const BasicString<CharT>&& str) noexcept`

Move assignment operator. Moves the value of an existing `BasicString` object to this instance.

##### `const CharT* GetRawString() const`

Returns a pointer to the raw character array.

##### `int GetLength() const`

Returns the length of the string.

##### `std::basic_string<CharT>& GetString()`

Returns a reference to the underlying `std::basic_string` object.

##### `BasicString<CharT> Clone() const`

Creates a new `BasicString` object with the same value as this instance.

##### `bool operator==(const BasicString<CharT>& str) const`

Compares this instance with another `BasicString` object for equality.

##### `CharT operator[](int index) const`

Gets the character at the specified index.

##### `int static Compare(const BasicString<CharT>& str1, int index1, const BasicString<CharT>& str2, int index2, int length, bool ignoreCase)`

Compares two specified `BasicString` objects.

##### `int CompareTo(const BasicString& str) const`

Compares this instance with a specified `BasicString` object.

##### `BasicString<CharT>& operator+(const BasicString<CharT>& str)`

Concatenates two `BasicString` objects.

##### `BasicString<CharT>& Concat(const std::initializer_list<BasicString<CharT>> strs)`

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

##### `int IndexOf(const BasicString<CharT>& substring)`

Returns the zero-based index of the first occurrence of a specified substring in this instance.

##### `int IndexOf(const BasicString<CharT>& substring, bool ignoreCase)`

Returns the zero-based index of the first occurrence of a specified substring in this instance, using a specified comparison option.

##### `int IndexOf(const BasicString<CharT>& substring, int startIndex, bool ignoreCase)`

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

##### `std::vector<BasicString<CharT>> Split(BasicString<CharT> separator [], int count, StringSplitOptions options)`

Splits a string into substrings that are based on the characters in an array.

### class `String`

Represents text as a sequence of UTF-16 code units (typedef of `BasicString<wchar_t>` or `BasicString<char>`).

#### Methods

```cpp
// Example usage of String class
#include <iostream>
#include <vector>
#include "DotNetDupe/String.h"

int main() {
    // Constructors
    DotNetDupe::System::String s1 = _T("Hello");
    DotNetDupe::System::String s2 = s1; // Copy constructor
    DotNetDupe::System::String s3 = _T(" World");

    std::wcout << L"s1: " << s1.GetRawString() << std::endl;
    std::wcout << L"s2 (copy of s1): " << s2.GetRawString() << std::endl;

    // Assignment operator
    DotNetDupe::System::String s4;
    s4 = s1;
    std::wcout << L"s4 (assigned from s1): " << s4.GetRawString() << std::endl;

    // Concatenation
    DotNetDupe::System::String s5 = s1 + s3;
    std::wcout << L"s1 + s3: " << s5.GetRawString() << std::endl;

    DotNetDupe::System::String s6 = _T("One");
    s6.Append(_T(' ')).Append(_T("Two")).Append(_T(" Three"));
    std::wcout << L"Appended string: " << s6.GetRawString() << std::endl;

    // Length
    std::wcout << L"Length of s5: " << s5.GetLength() << std::endl;

    // Comparison
    if (s1 == _T("Hello")) {
        std::wcout << L"s1 is 'Hello'" << std::endl;
    }

    DotNetDupe::System::String compareStr = _T("hello");
    if (s1.Equals(compareStr)) {
        std::wcout << L"s1 equals compareStr (case-sensitive)" << std::endl;
    } else {
        std::wcout << L"s1 does not equal compareStr (case-sensitive)" << std::endl;
    }

    // Contains
    if (s5.Contains(_T("World"))) {
        std::wcout << L"s5 contains 'World'" << std::endl;
    }

    // IndexOf
    std::wcout << L"Index of 'World' in s5: " << s5.IndexOf(_T("World")) << std::endl;

    // Substring (Remove)
    DotNetDupe::System::String sub = s5.Remove(5); // "Hello"
    std::wcout << L"s5.Remove(5): " << sub.GetRawString() << std::endl;

    // Replace
    DotNetDupe::System::String replaced = s5.Replace(_T('o'), _T('0'));
    std::wcout << L"s5.Replace('o', '0'): " << replaced.GetRawString() << std::endl;

    // Split
    DotNetDupe::System::String sentence = _T("This is a sample sentence.");
    std::vector<DotNetDupe::System::String> words = sentence.Split({_T(' ')}, 0, DotNetDupe::System::StringSplitOptions::None);
    std::wcout << L"Words in sentence:" << std::endl;
    for (const auto& word : words) {
        std::wcout << L"- " << word.GetRawString() << std::endl;
    }

    // Join
    DotNetDupe::System::String joined = DotNetDupe::System::String::Join(_T("-"), {s1, s3});
    std::wcout << L"Joined strings: " << joined.GetRawString() << std::endl;

    return 0;
}
```

## Namespace: `DotNetDupe::System`

### template class `BasicSystemException<CharT>`

Inherits from `BasicException<CharT>`. The base class for all predefined exceptions in the System namespace.

#### Methods

##### `BasicSystemException(const CharT* pchMessage)`

Initializes a new instance of the `BasicSystemException` class with a specified error message.

## Namespace: `DotNetDupe::System`

### interface `IClonable`

Defines a general-purpose mechanism for creating a new object that is a copy of the current object.

#### Methods

##### `virtual Object Clone() = 0`

Creates a new object that is a copy of the current instance.

## Namespace: `DotNetDupe::System`

### interface `IComparable`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Methods

##### `virtual int CompareTo(const Object& obj) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

## Namespace: `DotNetDupe::System`

### template interface `IComparable<T>`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Methods

##### `virtual int CompareTo<T>(const T& other) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

## Namespace: `DotNetDupe::System`

### interface `IFormatProvider<T>`

Provides a mechanism for retrieving a formatting service for a specified type.

#### Methods

##### `virtual Object* GetFormat(const T* formatType) = 0`

Returns an object that provides formatting services for the specified type.

## Namespace: `DotNetDupe::System`

### class `Object`

Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. This is the ultimate base class of all classes in the .NET Framework; it is the root of the type hierarchy.

#### Methods

##### `virtual bool operator == (const Object& obj) const`

Determines whether the specified `Object` is equal to the current `Object`.

##### `virtual bool Equals(const Object& obj)`

Determines whether the specified `Object` is equal to the current `Object`.

##### `static bool Equals(const Object& obj1, const Object& obj2)`

Determines whether the specified `Object` instances are considered equal.

##### `int GetHashCode()`

Serves as the default hash function.

##### `wchar_t* ToStringW()`

Returns a string that represents the current object (wide character version).

##### `char* ToStringA()`

Returns a string that represents the current object (ASCII character version).

## Struct: `template <class T> struct _init_list_with_indexer`

A helper struct to provide indexed access to `std::initializer_list`.

#### Members

##### `const std::initializer_list<T>& list`

The initializer list.

#### Methods

##### `_init_list_with_indexer(const std::initializer_list<T>& _list)`

Constructor. Initializes the struct with an initializer list.

##### `T operator[](unsigned int index)`

Provides indexed access to the elements of the initializer list.
