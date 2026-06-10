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
Guid g("12345678-1234-1234-1234-1234567890ab");
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

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `Guid`.

```cpp
#include "System/Guid.h"
#include "System/Console.h"
#include "System/String.h"
#include "System/Exception.h"
#include "System/Array.h"

using namespace DotNetDupe::System;

int main() {
    try {
        // Create a new random GUID
        Guid guidNew = Guid::NewGuid();
        Console::Write("New Guid: ");
        Console::WriteLine(guidNew.ToString());

        // Create GUID from string
        String guidString = "936da01f-9abd-4d9d-80c7-02af85c822a8";
        Guid guidFromString(guidString);
        Console::Write("Guid from string: ");
        Console::WriteLine(guidFromString.ToString());

        // Check Guid::Empty
        Guid guidEmpty = Guid::Empty;
        Console::Write("Empty Guid: ");
        Console::WriteLine(guidEmpty.ToString());

        // Compare GUIDs
        if (guidFromString == guidNew) {
            Console::WriteLine("GUIDs are equal (unexpected).");
        } else {
            Console::WriteLine("GUIDs are not equal.");
        }

        // Get byte representation
        Array<uint8_t> bytes = guidFromString.ToByteArray();
        Console::Write("Byte length: ");
        Console::WriteLine(bytes.GetLength());

    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


