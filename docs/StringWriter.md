### class `StringWriter`

Implements a `TextWriter` for writing information to a string. The information is stored in an underlying `StringBuilder`.

#### Methods

##### `StringWriter()`

Initializes a new instance of the `StringWriter` class.

**Usage:**
```cpp
StringWriter writer;
```

##### `StringWriter(Text::StringBuilder& sb)`

Initializes a new instance of the `StringWriter` class that writes to the specified `StringBuilder`.

**Usage:**
```cpp
StringBuilder sb;
StringWriter writer(sb);
```

##### `void Write(wchar_t value) override`

Writes a character to the string.

**Usage:**
```cpp
writer.Write(L'A');
```

##### `String ToString() const override`

Returns the string containing the characters written to the current `StringWriter` so far.

**Usage:**
```cpp
String s = writer.ToString();
```
