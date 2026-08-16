# BitConverter

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/BitConverter.h"`

Converts base data types to an array of bytes, and an array of bytes to base data types with endianness support.

---

## Syntax

```cpp
class BitConverter : public Object;
```

---

## Static Fields

### `static const bool IsLittleEndian`
Indicates the byte order ("endianness") in which data is stored on the current computer architecture.

---

## Static Methods

### `static Array<byte> GetBytes(int iValue)` / `short` / `long long` / `float` / `double` / `bool`
Returns the specified primitive value as an array of bytes.

```cpp
Array<byte> bytes = BitConverter::GetBytes(1024);
```

### `static int ToInt32(Array<byte>& arrValue, int iStartIndex)`
Returns a 32-bit signed integer converted from four bytes at a specified position in a byte array.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `iStartIndex` is out of bounds or insufficient bytes remain.

```cpp
int val = BitConverter::ToInt32(bytes, 0); // 1024
```

### `static double ToDouble(Array<byte>& arrValue, int iStartIndex)`
Returns a double-precision floating-point number converted from eight bytes at a specified position in a byte array.

### `static String ToString(Array<byte>& arrValue, int iStartIndex = 0, int iLength = -1)`
Converts the numeric value of each element of a specified array of bytes to its equivalent hexadecimal string representation (e.g. `00-04-00-00`).

```cpp
String hex = BitConverter::ToString(bytes); // "00-04-00-00"
```

### `static long long DoubleToInt64Bits(double dValue)` / `static double Int64BitsToDouble(long long llValue)`
Reinterprets double bits as 64-bit integer bits and vice-versa.

---

## Example

```cpp
#include "System/Console.h"
#include "System/BitConverter.h"

using namespace DotNetDupe::System;

int main() {
    Console::WriteLine("Architecture LittleEndian: {0}", BitConverter::IsLittleEndian);

    int number = 16909060; // 0x01020304
    Array<byte> bytes = BitConverter::GetBytes(number);

    Console::WriteLine("Hex: {0}", BitConverter::ToString(bytes));

    int restored = BitConverter::ToInt32(bytes, 0);
    Console::WriteLine("Restored Int32: {0}", restored);

    return 0;
}
```
