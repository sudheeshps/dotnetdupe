### class `StringWriter`

Implements a `TextWriter` for writing information to a string. The information is stored in an underlying `StringBuilder`.

#### Methods

##### `StringWriter()`

Initializes a new instance of the `StringWriter` class.

**Usage:**
```cpp
StringWriter swWriter;
```

##### `StringWriter(Text::StringBuilder& sbBuilder)`

Initializes a new instance of the `StringWriter` class that writes to the specified `StringBuilder`.

**Usage:**
```cpp
StringBuilder sbBuilder;
StringWriter swWriter(sbBuilder);
```

##### `void Write(wchar_t cValue) override`

Writes a character to the string.

**Usage:**
```cpp
swWriter.Write(L'A');
```

##### `String ToString() const override`

Returns the string containing the characters written to the current `StringWriter` so far.

**Usage:**
```cpp
String sResult = swWriter.ToString();
```

