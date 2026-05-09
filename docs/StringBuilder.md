### class `StringBuilder`

Represents a mutable string of characters.

#### Methods

##### `StringBuilder()`

Initializes a new instance of the `StringBuilder` class.

**Usage:**
```cpp
StringBuilder sb;
```

##### `StringBuilder(const String& value)`

Initializes a new instance of the `StringBuilder` class using the specified string.

**Usage:**
```cpp
StringBuilder sb(_T("Initial"));
```

##### `StringBuilder& Append(...)`

Appends the string representation of a specified value to this instance. Overloads are available for `String`, `TCHAR*`, `TCHAR`, `int`, `long long`, `double`, and `bool`.

**Usage:**
```cpp
sb.Append(_T("Items: ")).Append(10);
```

##### `StringBuilder& AppendLine(...)`

Appends the string representation of a specified value followed by the default line terminator.

**Usage:**
```cpp
sb.AppendLine(_T("Line 1"));
```

##### `StringBuilder& Clear()`

Removes all characters from the current `StringBuilder` instance.

**Usage:**
```cpp
sb.Clear();
```

##### `int GetLength() const`

Gets the length of the current `StringBuilder` object.

**Usage:**
```cpp
int len = sb.GetLength();
```

##### `void SetLength(int value)`

Sets the length of the current `StringBuilder` object.

**Usage:**
```cpp
sb.SetLength(0);
```

##### `String ToString() const`

Converts the value of this instance to a `String`.

**Usage:**
```cpp
String result = sb.ToString();
```
