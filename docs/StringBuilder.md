# StringBuilder

**Namespace:** `DotNetDupe::System::Text`  
**Header:** `#include "System/Text/StringBuilder.h"`

Represents a mutable string of characters. Use `StringBuilder` to modify a string without creating intermediate heap allocations.

---

## Syntax

```cpp
class StringBuilder : public Object;
```

---

## Constructors

### `StringBuilder()`
Initializes a new, empty instance of the `StringBuilder` class with default capacity.

### `StringBuilder(int capacity)`
Initializes a new instance of the `StringBuilder` class using the specified capacity.

### `StringBuilder(const String& value)`
Initializes a new instance of the `StringBuilder` class using the specified string.

---

## Properties

- `int GetLength() const` / `void SetLength(int value)`: Gets or sets the length of the current `StringBuilder` object.
- `int GetCapacity() const` / `void SetCapacity(int value)`: Gets or sets the maximum number of characters that can be contained in the memory allocated by the current instance.

---

## Member Functions

### `StringBuilder& Append(const String& / const char* / char / int / long long / double / bool value)`
Appends a string or string representation of a specified primitive to the end of this instance. Returns a reference to this instance for chaining.

### `StringBuilder& AppendLine()`
Appends the default line terminator to the end of the current `StringBuilder` object.

### `StringBuilder& AppendLine(const String& value)`
Appends a copy of the specified string followed by the default line terminator to the end of the current `StringBuilder` object.

### `StringBuilder& Clear()`
Removes all characters from the current `StringBuilder` instance.

### `String ToString() const`
Converts the value of this instance to a `String`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Text/StringBuilder.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

int main() {
    StringBuilder sb(256);
    sb.AppendLine("=== System Report ===");
    sb.Append("Active Users: ").Append(42).AppendLine();
    sb.Append("CPU Load: ").Append(12.5).Append("%").AppendLine();
    sb.Append("Healthy: ").Append(true);

    String finalOutput = sb.ToString();
    Console::WriteLine(finalOutput);

    return 0;
}
```
