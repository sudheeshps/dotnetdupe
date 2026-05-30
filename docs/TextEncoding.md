### class `Encoding`

Abstract base class for character encodings.

#### Methods

##### `virtual Array<char> GetBytes(const String& s)`

When overridden in a derived class, encodes all the characters in the specified string into a sequence of bytes.

**Usage:**
```cpp
auto bytes = encoding->GetBytes("Hello");
```

##### `virtual String GetString(const Array<char>& bytes)`

When overridden in a derived class, decodes all the bytes in the specified byte array into a string.

**Usage:**
```cpp
String s = encoding->GetString(bytes);
```

### class `TextEncoding`

Provides static factory methods for standard encodings.

#### Methods

##### `static std::shared_ptr<Encoding> UTF8()`

Returns an encoding for the UTF-8 format.

**Usage:**
```cpp
auto encoding = TextEncoding::UTF8();
```

