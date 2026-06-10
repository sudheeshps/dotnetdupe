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
StringBuilder sb("Initial");
```

##### `StringBuilder& Append(...)`

Appends the string representation of a specified value to this instance. Overloads are available for `String`, `TCHAR*`, `TCHAR`, `int`, `long long`, `double`, and `bool`.

**Usage:**
```cpp
sb.Append("Items: ").Append(10);
```

##### `StringBuilder& AppendLine(...)`

Appends the string representation of a specified value followed by the default line terminator.

**Usage:**
```cpp
sb.AppendLine("Line 1");
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

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `StringBuilder`.

```cpp
#include "System/Text/StringBuilder.h"
#include "System/Console.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

int main() {
    try {
        StringBuilder sb;
        
        // Chained Appends
        sb.Append("Value: ")
          .Append(42)
          .Append(", Status: ")
          .Append(true)
          .AppendLine()
          .AppendLine("Next line of data.");
          
        Console::WriteLine("StringBuilder result:");
        Console::WriteLine(sb.ToString());
        
        // Displaying length
        Console::Write("Buffer length: ");
        Console::WriteLine(sb.GetLength());
        
        // Clearing
        sb.Clear();
        sb.Append("Brand new content");
        Console::WriteLine(sb.ToString());
    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


