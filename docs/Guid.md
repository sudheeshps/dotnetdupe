# Guid

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Guid.h"`

Represents a Globally Unique Identifier (GUID / UUID).

---

## Syntax

```cpp
class Guid : public Object;
```

---

## Static Fields & Methods

### `static const Guid Empty`
A read-only instance of the `Guid` structure whose value is all zeros (`00000000-0000-0000-0000-000000000000`).

### `static Guid NewGuid()`
Generates a new RFC-4122 compliant version 4 random `Guid`.

```cpp
Guid objId = Guid::NewGuid();
```

---

## Constructors

### `Guid()`
Initializes a new `Guid` structure equal to `Guid::Empty`.

### `Guid(const String& g)`
Initializes a `Guid` from a hyphenated or unhyphenated hex string.

- **Throws:**
  - `FormatException`: If the string is not in a valid GUID format.

### `Guid(const Array<uint8_t>& b)`
Initializes a `Guid` from a 16-element byte array.

---

## Member Functions & Operators

### `String ToString() const`
Returns a string representation of the value of this instance in standard `8-4-4-4-12` format.

```cpp
String sId = objId.ToString(); // "d3b07384-d113-40e1-95c5-7f4c029b936d"
```

### `Array<uint8_t> ToByteArray() const`
Returns a 16-element byte array that contains the value of this instance.

### `bool operator==(const Guid& other) const`
Returns `true` if this instance and `other` represent the same GUID value.

### `bool operator!=(const Guid& other) const`
Returns `true` if this instance and `other` represent different GUID values.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Guid.h"

using namespace DotNetDupe::System;

int main() {
    Guid objId1 = Guid::NewGuid();
    Guid objId2 = Guid::NewGuid();

    Console::WriteLine("Generated GUID 1: {0}", objId1.ToString());
    Console::WriteLine("Generated GUID 2: {0}", objId2.ToString());
    Console::WriteLine("Are Equal: {0}", objId1 == objId2);

    return 0;
}
```
