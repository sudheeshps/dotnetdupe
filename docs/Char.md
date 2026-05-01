### template class `BasicChar<CharT>`

Represents a character as a UTF-16 code unit. The `Char` class is a typedef for `BasicChar<TCHAR>`.

#### Methods

##### `BasicChar(CharT c)`

Initializes a new instance of the `BasicChar` class with the specified character.

**Parameters:**
- `c`: A character.

**Usage:**
```cpp
Char c(_T('A'));
```

##### `void operator= (CharT c)`

Assigns a character to the `BasicChar` object.

**Parameters:**
- `c`: The character to assign.

**Usage:**
```cpp
Char c = _T('A');
c = _T('B');
```

##### `bool Equals(CharT c)`

Indicates whether this instance and a specified character are equal.

**Parameters:**
- `c`: The character to compare with.

**Returns:**
- `true` if `c` is the same as the character in this instance; otherwise, `false`.

**Usage:**
```cpp
Char c(_T('A'));
bool result = c.Equals(_T('A')); // true
```

##### `bool operator == (CharT c)`

Compares this instance to a specified character for equality.

**Parameters:**
- `c`: The character to compare.

**Returns:**
- `true` if this instance is equal to `c`; otherwise, `false`.

**Usage:**
```cpp
Char c(_T('A'));
if (c == _T('A')) { /* ... */ }
```

##### `double static GetNumericValue(CharT c)`

Converts the specified numeric Unicode character to a double-precision floating-point number.

**Parameters:**
- `c`: The Unicode character to convert.

**Returns:**
- The numeric value of `c` if that character represents a number; otherwise, -1.0.

**Usage:**
```cpp
double val = Char::GetNumericValue(_T('5')); // 5.0
```

##### `bool static IsAscii(CharT c)`

Indicates whether the specified character is an ASCII character.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is an ASCII character; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsAscii(_T('A')); // true
```

##### `bool static IsAsciiDigit(CharT c)`

Indicates whether the specified character is an ASCII digit.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is an ASCII digit; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsAsciiDigit(_T('7')); // true
```

##### `bool static IsBetween(CharT c, CharT minInclusive, CharT maxInclusive)`

Indicates whether the specified character is between the specified minimum and maximum inclusive values.

**Parameters:**
- `c`: The character to evaluate.
- `minInclusive`: The lower bound of the range.
- `maxInclusive`: The upper bound of the range.

**Returns:**
- `true` if `c` is within the range; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsBetween(_T('M'), _T('A'), _T('Z')); // true
```

##### `bool static IsControl(CharT c)`

Indicates whether the specified character is a control character.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is a control character; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsControl(_T('\n')); // true
```

##### `bool static IsDigit(CharT c)`

Indicates whether the specified character is a decimal digit.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is a decimal digit; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsDigit(_T('5')); // true
```

##### `bool static IsLower(CharT c)`

Indicates whether the specified character is a lowercase letter.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is a lowercase letter; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsLower(_T('a')); // true
```

##### `bool static IsLetter(CharT c)`

Indicates whether the specified character is a letter.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is a letter; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsLetter(_T('A')); // true
```

##### `bool static IsLetterOrDigit(CharT c)`

Indicates whether the specified character is a letter or a decimal digit.

**Parameters:**
- `c`: The character to evaluate.

**Returns:**
- `true` if `c` is a letter or a digit; otherwise, `false`.

**Usage:**
```cpp
bool result = Char::IsLetterOrDigit(_T('A')); // true
```

##### `CharT static ToLower(CharT c)`

Converts the value of a specified Unicode character to its lowercase equivalent.

**Parameters:**
- `c`: The character to convert.

**Returns:**
- The lowercase equivalent of `c`, or the unchanged value of `c`, if `c` is already lowercase or not a letter.

**Usage:**
```cpp
TCHAR lower = Char::ToLower(_T('A')); // 'a'
```

##### `CharT GetChar()`

Returns the character value of this instance.

**Returns:**
- The character value of this instance.

**Usage:**
```cpp
Char c(_T('X'));
TCHAR val = c.GetChar(); // 'X'
```
