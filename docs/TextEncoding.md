### class `TextEncoding`

Represents a character encoding.

#### Methods

##### `static std::shared_ptr<TextEncoding> GetUTF8()`

Returns an encoding for the UTF-8 format.

**Usage:**
```cpp
auto encoding = TextEncoding::GetUTF8();
```

##### `virtual std::vector<uint8_t> GetBytes(const String& s) const`

When overridden in a derived class, encodes all the characters in the specified string into a sequence of bytes.

**Usage:**
```cpp
auto bytes = encoding->GetBytes(_T("Hello"));
```

##### `virtual String GetString(const std::vector<uint8_t>& bytes) const`

When overridden in a derived class, decodes all the bytes in the specified byte array into a string.

**Usage:**
```cpp
String s = encoding->GetString(bytes);
```
