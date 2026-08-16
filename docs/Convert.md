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

### Boolean Conversions
- `static bool ToBoolean(bool value)`
- `static bool ToBoolean(int value)`
- `static bool ToBoolean(long long value)`
- `static bool ToBoolean(double value)`
- `static bool ToBoolean(const String& value)`
- `static bool ToBoolean(const char* value)`

### Integer Conversions (Signed)
- `static signed char ToSByte(...)` (from `bool`, `unsigned char`, `signed char`, `short`, `int`, `long long`, `double`, `const String&`, `const char*`)
- `static short ToInt16(...)` (from primitive types, strings, and with radix `fromBase`)
- `static int ToInt32(...)` (from primitive types, strings, and with radix `fromBase`)
- `static long long ToInt64(...)` (from primitive types, strings, and with radix `fromBase`)

### Integer Conversions (Unsigned)
- `static unsigned char ToByte(...)` (from primitive types, strings, and with radix `fromBase`)
- `static unsigned short ToUInt16(...)` (from primitive types and strings)
- `static unsigned int ToUInt32(...)` (from primitive types and strings)
- `static unsigned long long ToUInt64(...)` (from primitive types and strings)

### Floating-Point Conversions
- `static float ToSingle(...)` (from `int`, `long long`, `double`, `const String&`, `const char*`)
- `static double ToDouble(...)` (from `int`, `long long`, `float`, `const String&`, `const char*`)

### Character Conversions
- `static char ToChar(unsigned short value)`
- `static char ToChar(int value)`
- `static char ToChar(long long value)`
- `static char ToChar(const String& value)`
- `static char ToChar(const char* value)`

### Base64 Encoding & Decoding
- `static String ToBase64String(const Array<char>& inArray)`: Converts an array of 8-bit integers to its equivalent string representation that is encoded with base-64 digits.
- `static Array<char> FromBase64String(const String& s)`: Converts the specified base-64 encoded string to an array of bytes.
- `static Array<char> FromBase64String(const char* s)`: Converts the specified C-string to an array of bytes.

### String Conversions
- `static String ToString(...)` (overloads for `bool`, `char`, `signed char`, `unsigned char`, `short`, `int`, `long long`, `float`, `double`, `unsigned short`, `unsigned int`, `unsigned long long`)
- `static String ToString(int value, int toBase)` (radix base 2, 8, 10, 16)
- `static String ToString(long long value, int toBase)` (radix base 2, 8, 10, 16)

---

## Example

```cpp
#include "System/Console.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;

int main() {
    String sBin = "11110000";
    int iVal = Convert::ToInt32(sBin, 2);
    Console::WriteLine("Binary {0} = Decimal {1}", sBin, iVal);

    String sHex = Convert::ToString(iVal, 16);
    Console::WriteLine("Decimal {0} = Hex {1}", iVal, sHex);

    return 0;
}
```
