### template class `BasicString<CharT>`

Represents text as a sequence of character code units. The `String` class is a typedef for `BasicString<TCHAR>`.

#### Methods

##### `BasicString(const CharT* pStr)`

Initializes a new instance of the `BasicString` class with a pointer to a null-terminated character array.

**Parameters:**
- `pStr`: A pointer to a null-terminated character array.

**Usage:**
```cpp
String s(_T("Hello"));
```

##### `const CharT* GetRawString() const`

Returns a pointer to the raw character array.

**Returns:**
- A pointer to the underlying null-terminated character array.

**Usage:**
```cpp
String s(_T("Hello"));
const TCHAR* raw = s.GetRawString();
```

##### `int GetLength() const`

Returns the length of the string.

**Returns:**
- The number of characters in the string.

**Usage:**
```cpp
String s(_T("Hello"));
int len = s.GetLength(); // 5
```

##### `BasicString<CharT> Clone() const`

Creates a new `BasicString` object with the same value as this instance.

**Returns:**
- A new `BasicString` object that is a copy of this instance.

**Usage:**
```cpp
String s1(_T("Hello"));
String s2 = s1.Clone();
```

##### `bool operator==(const BasicString<CharT>& str) const`

Compares this instance with another `BasicString` object for equality.

**Parameters:**
- `str`: The string to compare with.

**Returns:**
- `true` if the strings are equal; otherwise, `false`.

**Usage:**
```cpp
String s1(_T("abc")), s2(_T("abc"));
if (s1 == s2) { /* ... */ }
```

##### `CharT operator[](int index) const`

Gets the character at the specified index.

**Parameters:**
- `index`: The zero-based position of the character to retrieve.

**Returns:**
- The character at the specified index.

**Usage:**
```cpp
String s(_T("Hello"));
TCHAR c = s[1]; // 'e'
```

##### `int static Compare(const BasicString<CharT>& str1, int index1, const BasicString<CharT>& str2, int index2, int length, bool ignoreCase)`

Compares two specified `BasicString` objects.

**Parameters:**
- `str1`: The first string to compare.
- `index1`: The position of the first substring within `str1`.
- `str2`: The second string to compare.
- `index2`: The position of the second substring within `str2`.
- `length`: The number of characters to compare.
- `ignoreCase`: `true` to ignore case during the comparison; otherwise, `false`.

**Returns:**
- An integer that indicates the lexical relationship between the two substrings.

**Usage:**
```cpp
int result = String::Compare(s1, 0, s2, 0, 5, true);
```

##### `BasicString<CharT> operator+(const BasicString<CharT>& str) const`

Concatenates two `BasicString` objects.

**Parameters:**
- `str`: The string to append.

**Returns:**
- A new string that is the concatenation of the current instance and `str`.

**Usage:**
```cpp
String s1(_T("Hello")), s2(_T(" World"));
String s3 = s1 + s2; // "Hello World"
```

##### `bool Contains(CharT c) const`

Determines whether a character occurs within this string.

**Parameters:**
- `c`: The character to seek.

**Returns:**
- `true` if the character is found; otherwise, `false`.

**Usage:**
```cpp
String s(_T("Hello"));
bool hasE = s.Contains(_T('e')); // true
```

##### `bool Contains(const BasicString<CharT>& str)`

Determines whether a specified substring occurs within this string.

**Parameters:**
- `str`: The substring to seek.

**Returns:**
- `true` if the substring is found; otherwise, `false`.

**Usage:**
```cpp
String s(_T("Hello World"));
bool hasWorld = s.Contains(_T("World")); // true
```

##### `void CopyTo(int sourceIndex, CharT* pDestination, int destinationIndex, int destArraySize, int count) const`

Copies a specified number of characters from a specified position in this instance to a specified position in a character array.

**Parameters:**
- `sourceIndex`: The character position in this instance from which characters will be copied.
- `pDestination`: An array of characters to which characters in this instance will be copied.
- `destinationIndex`: The starting index in `pDestination` at which characters will be copied.
- `destArraySize`: The size of the destination array.
- `count`: The number of characters to copy.

**Usage:**
```cpp
TCHAR buffer[10];
s.CopyTo(0, buffer, 0, 10, 5);
```

##### `bool EndsWith(const BasicString<CharT>& suffix, bool ignoreCase) const`

Determines whether the end of this string instance matches the specified string.

**Parameters:**
- `suffix`: The string to compare to the substring at the end of this instance.
- `ignoreCase`: `true` to ignore case during the comparison; otherwise, `false`.

**Returns:**
- `true` if `suffix` matches the end of this instance; otherwise, `false`.

**Usage:**
```cpp
String s(_T("filename.txt"));
bool isTxt = s.EndsWith(_T(".txt"), true); // true
```

##### `int IndexOf(const BasicString<CharT>& substring, int startIndex, bool ignoreCase) const`

Returns the zero-based index of the first occurrence of a specified substring in this instance.

**Parameters:**
- `substring`: The string to seek.
- `startIndex`: The search starting position.
- `ignoreCase`: `true` to ignore case during the comparison; otherwise, `false`.

**Returns:**
- The zero-based index position of `substring` if it is found, or -1 if it is not.

**Usage:**
```cpp
String s(_T("Hello World"));
int index = s.IndexOf(_T("World"), 0, false); // 6
```

##### `BasicString<CharT>& Append(const BasicString<CharT>& str)`

Appends a `BasicString` object to this instance.

**Parameters:**
- `str`: The string to append.

**Returns:**
- A reference to this instance after the append operation.

**Usage:**
```cpp
String s(_T("Hello"));
s.Append(_T(" World"));
```

##### `BasicString<CharT>& Insert(int index, const BasicString<CharT>& str)`

Inserts a specified string at a specified index position in this instance.

**Parameters:**
- `index`: The zero-based index at which to insert.
- `str`: The string to insert.

**Returns:**
- A reference to this instance after the insertion.

**Usage:**
```cpp
String s(_T("Hello World"));
s.Insert(5, _T(",")); // "Hello, World"
```

##### `bool IsEmpty() const`

Indicates whether this string is empty.

**Returns:**
- `true` if the length of the string is 0; otherwise, `false`.

**Usage:**
```cpp
String s(_T(""));
if (s.IsEmpty()) { /* ... */ }
```

##### `BasicString<CharT> static Join(const BasicString<CharT>& separator, std::initializer_list<BasicString<CharT>> strings)`

Concatenates the elements of a collection, using the specified separator string between each element.

**Parameters:**
- `separator`: The string to use as a separator.
- `strings`: A collection that contains the strings to concatenate.

**Returns:**
- A string that consists of the members of `strings` delimited by `separator`.

**Usage:**
```cpp
String result = String::Join(_T(", "), { _T("apple"), _T("orange"), _T("banana") });
```

##### `BasicString<CharT> PadLeft(int totalWidth, CharT ch)`

Returns a new string that right-aligns the characters in this instance by padding them on the left with a specified character, for a specified total length.

**Parameters:**
- `totalWidth`: The number of characters in the resulting string.
- `ch`: A padding character.

**Returns:**
- A new string that is equivalent to this instance, but right-aligned and padded on the left.

**Usage:**
```cpp
String s(_T("123"));
String padded = s.PadLeft(5, _T('0')); // "00123"
```

##### `BasicString<CharT> Remove(int startIndex, int count) const`

Deletes a specified number of characters from this instance beginning at a specified position.

**Parameters:**
- `startIndex`: The zero-based position to begin deleting.
- `count`: The number of characters to delete.

**Returns:**
- A new string that is equivalent to this instance less the removed characters.

**Usage:**
```cpp
String s(_T("Hello World"));
String result = s.Remove(5, 6); // "Hello"
```

##### `BasicString<CharT> Replace(const BasicString<CharT>& orignalStr, const BasicString<CharT>& replaceStr)`

Returns a new string in which all occurrences of a specified string in the current instance are replaced with another specified string.

**Parameters:**
- `orignalStr`: The string to be replaced.
- `replaceStr`: The string to replace all occurrences of `orignalStr`.

**Returns:**
- A string that is equivalent to the current string except that all occurrences of `orignalStr` are replaced with `replaceStr`.

**Usage:**
```cpp
String s(_T("Hello World"));
String result = s.Replace(_T("World"), _T("DotNet")); // "Hello DotNet"
```

##### `std::vector<BasicString<CharT>> Split(CharT separator)`

Splits a string into substrings based on a separator character.

**Parameters:**
- `separator`: A character that delimits the substrings in this string.

**Returns:**
- A vector whose elements contain the substrings in this instance that are delimited by `separator`.

**Usage:**
```cpp
String s(_T("apple,orange,banana"));
auto parts = s.Split(_T(','));
```

##### `bool StartsWith(const BasicString<CharT>& prefix, bool ignoreCase) const`

Determines whether the beginning of this string instance matches the specified string.

**Parameters:**
- `prefix`: The string to compare.
- `ignoreCase`: `true` to ignore case during the comparison; otherwise, `false`.

**Returns:**
- `true` if `prefix` matches the beginning of this string; otherwise, `false`.

**Usage:**
```cpp
String s(_T("Hello World"));
bool result = s.StartsWith(_T("Hello"), false); // true
```

##### `BasicString<CharT> Substring(int startIndex, int length) const`

Retrieves a substring from this instance.

**Parameters:**
- `startIndex`: The zero-based starting character position of a substring in this instance.
- `length`: The number of characters in the substring.

**Returns:**
- A string that is equivalent to the substring of length `length` that begins at `startIndex` in this instance.

**Usage:**
```cpp
String s(_T("Hello World"));
String sub = s.Substring(0, 5); // "Hello"
```
