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

### `static Array<byte> GetBytes(...)`
Returns the specified primitive value as an array of bytes.

Overloads provided:
- `static Array<byte> GetBytes(bool bValue)`
- `static Array<byte> GetBytes(char chValue)`
- `static Array<byte> GetBytes(short iValue)`
- `static Array<byte> GetBytes(int iValue)`
- `static Array<byte> GetBytes(long long llValue)`
- `static Array<byte> GetBytes(float fValue)`
- `static Array<byte> GetBytes(double dValue)`
- `static Array<byte> GetBytes(unsigned short iValue)`
- `static Array<byte> GetBytes(unsigned int iValue)`
- `static Array<byte> GetBytes(unsigned long long llValue)`

```cpp
Array<byte> arrBytes = BitConverter::GetBytes(1024);
```

---

### Conversion from Byte Arrays

### `static bool ToBoolean(Array<byte>& arrValue, int iStartIndex)`
Returns a boolean value converted from one byte at a specified position in a byte array.

### `static char ToChar(Array<byte>& arrValue, int iStartIndex)`
Returns a character converted from one byte at a specified position in a byte array.

### `static short ToInt16(Array<byte>& arrValue, int iStartIndex)`
Returns a 16-bit signed integer converted from two bytes at a specified position in a byte array.

### `static int ToInt32(Array<byte>& arrValue, int iStartIndex)`
Returns a 32-bit signed integer converted from four bytes at a specified position in a byte array.

### `static long long ToInt64(Array<byte>& arrValue, int iStartIndex)`
Returns a 64-bit signed integer converted from eight bytes at a specified position in a byte array.

### `static float ToSingle(Array<byte>& arrValue, int iStartIndex)`
Returns a single-precision floating-point number converted from four bytes at a specified position in a byte array.

### `static double ToDouble(Array<byte>& arrValue, int iStartIndex)`
Returns a double-precision floating-point number converted from eight bytes at a specified position in a byte array.

### `static unsigned short ToUInt16(Array<byte>& arrValue, int iStartIndex)`
Returns a 16-bit unsigned integer converted from two bytes at a specified position in a byte array.

### `static unsigned int ToUInt32(Array<byte>& arrValue, int iStartIndex)`
Returns a 32-bit unsigned integer converted from four bytes at a specified position in a byte array.

### `static unsigned long long ToUInt64(Array<byte>& arrValue, int iStartIndex)`
Returns a 64-bit unsigned integer converted from eight bytes at a specified position in a byte array.

---

### String Representation

### `static String ToString(Array<byte>& arrValue)`
Converts the numeric value of each element of a specified array of bytes to its equivalent hexadecimal string representation delimited by hyphens (e.g., `00-04-00-00`).

### `static String ToString(Array<byte>& arrValue, int iStartIndex)`
Converts the numeric value of each element of a specified subarray of bytes starting at `iStartIndex` to its equivalent hexadecimal string representation.

### `static String ToString(Array<byte>& arrValue, int iStartIndex, int iLength)`
Converts the numeric value of each element of a specified subarray of bytes starting at `iStartIndex` for `iLength` bytes to its equivalent hexadecimal string representation.

---

### Bit Level Reinterpretations

- `static long long DoubleToInt64Bits(double dValue)`
- `static double Int64BitsToDouble(long long llValue)`
- `static int SingleToInt32Bits(float fValue)`
- `static float Int32BitsToSingle(int iValue)`

---

## Example

```cpp
#include "System/Console.h"
#include "System/BitConverter.h"

using namespace DotNetDupe::System;

int main() {
    Console::WriteLine("Architecture LittleEndian: {0}", BitConverter::IsLittleEndian);

    int iNumber = 16909060; // 0x01020304
    Array<byte> arrBytes = BitConverter::GetBytes(iNumber);

    Console::WriteLine("Hex: {0}", BitConverter::ToString(arrBytes));

    int iRestored = BitConverter::ToInt32(arrBytes, 0);
    Console::WriteLine("Restored Int32: {0}", iRestored);

    return 0;
}
```
