# Convert

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Convert.h"`

Converts a base data type to another base data type, parsing strings with radix bases (2, 8, 10, 16) and enforcing range checks.

---

## Syntax

```cpp
class Convert : public Object;
```

---

## Static Methods

### `static int ToInt32(const String& value, int fromBase = 10)`
Converts the string representation of a number in a specified base to an equivalent 32-bit signed integer.

- **Parameters:**
  - `value` (`const String&`): String containing the number to convert.
  - `fromBase` (`int`): Radix base (`2`, `8`, `10`, or `16`).
- **Throws:**
  - `FormatException`: If `value` is not in a recognized format.
  - `OverflowException`: If the number falls outside Int32 bounds.

```cpp
int hexVal = Convert::ToInt32("0xFF", 16); // 255
int binVal = Convert::ToInt32("1010", 2);   // 10
```

### `static bool ToBoolean(const String& value)`
Converts the specified string representation of a logical value (`"True"`, `"False"`, `"1"`, `"0"`) to its Boolean equivalent.

```cpp
bool b = Convert::ToBoolean("True"); // true
```

### `static double ToDouble(const String& value)` / `float ToSingle(const String& value)`
Converts a string representation of a floating-point number to its double/single precision equivalent.

```cpp
double d = Convert::ToDouble("3.14159");
```

### `static String ToString(int value, int toBase = 10)`
Converts the value of a 32-bit signed integer to its equivalent string representation in a specified radix base.

```cpp
String sHex = Convert::ToString(255, 16); // "ff"
String sBin = Convert::ToString(10, 2);   // "1010"
```

---

## Example

```cpp
#include "System/Console.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;

int main() {
    String sBin = "11110000";
    int val = Convert::ToInt32(sBin, 2);
    Console::WriteLine("Binary {0} = Decimal {1}", sBin, val);

    String sHex = Convert::ToString(val, 16);
    Console::WriteLine("Decimal {0} = Hex {1}", val, sHex);

    return 0;
}
```
