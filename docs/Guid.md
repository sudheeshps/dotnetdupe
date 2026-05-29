### class `Guid`

Represents a globally unique identifier (GUID).

#### Methods

##### `Guid()`

Initializes a new instance of the `Guid` class. The default value is `Guid::Empty`.

**Usage:**
```cpp
Guid g;
```

##### `Guid(const String& g)`

Initializes a new instance of the `Guid` class using the value represented by the specified string.

**Usage:**
```cpp
Guid g(_T("12345678-1234-1234-1234-1234567890ab"));
```

##### `static Guid NewGuid()`

Initializes a new instance of the `Guid` class with a random unique identifier.

**Usage:**
```cpp
Guid g = Guid::NewGuid();
```

##### `String ToString() const`

Returns a string representation of the value of this instance of the `Guid` class.

**Usage:**
```cpp
String s = g.ToString();
```

##### `Array<uint8_t> ToByteArray() const`

Returns a 16-element byte array that contains the value of this instance.

**Usage:**
```cpp
auto bytes = g.ToByteArray();
```

##### `bool operator==(const Guid& other) const`

Returns a value indicating whether this instance and a specified `Guid` object represent the same value.

**Usage:**
```cpp
if (g1 == g2) { /* ... */ }
```

#### Fields

##### `static const Guid Empty`

A read-only instance of the `Guid` class whose value is all zeros.
