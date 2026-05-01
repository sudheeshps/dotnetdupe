### class `BitConverter`

Converts base data types to an array of bytes, and an array of bytes to base data types.

#### Fields

##### `static const bool IsLittleEndian`

Indicates the byte order ("endianness") in which data is stored in this computer architecture.

**Usage:**
```cpp
if (BitConverter::IsLittleEndian) {
    std::cout << "System is Little Endian" << std::endl;
}
```

#### Methods

##### `static Array<byte> GetBytes(bool value)`

Returns the specified Boolean value as an array of bytes.

**Parameters:**
- `value`: A Boolean value.

**Returns:**
- An array of bytes with length 1.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(true);
```

##### `static Array<byte> GetBytes(TCHAR value)`

Returns the specified Unicode character value as an array of bytes.

**Parameters:**
- `value`: A Unicode character.

**Returns:**
- An array of bytes with length 2.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(_T('A'));
```

##### `static Array<byte> GetBytes(double value)`

Returns the specified double-precision floating-point value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 8.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(3.14159);
```

##### `static Array<byte> GetBytes(short value)`

Returns the specified 16-bit signed integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 2.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes((short)123);
```

##### `static Array<byte> GetBytes(int value)`

Returns the specified 32-bit signed integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 4.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456789);
```

##### `static Array<byte> GetBytes(long long value)`

Returns the specified 64-bit signed integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 8.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456789012345LL);
```

##### `static Array<byte> GetBytes(float value)`

Returns the specified single-precision floating-point value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 4.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(3.14f);
```

##### `static Array<byte> GetBytes(unsigned short value)`

Returns the specified 16-bit unsigned integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 2.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes((unsigned short)123);
```

##### `static Array<byte> GetBytes(unsigned int value)`

Returns the specified 32-bit unsigned integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 4.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456789U);
```

##### `static Array<byte> GetBytes(unsigned long long value)`

Returns the specified 64-bit unsigned integer value as an array of bytes.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- An array of bytes with length 8.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456789012345ULL);
```

##### `static bool ToBoolean(Array<byte>& value, int startIndex)`

Returns a Boolean value converted from one byte at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- `true` if the byte at `startIndex` in value is nonzero; otherwise, `false`.

**Usage:**
```cpp
Array<byte> bytes = { 0, 1, 0 };
bool result = BitConverter::ToBoolean(bytes, 1); // true
```

##### `static TCHAR ToChar(Array<byte>& value, int startIndex)`

Returns a Unicode character converted from two bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array.
- `startIndex`: The starting position within value.

**Returns:**
- A character formed by two bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(_T('Z'));
TCHAR result = BitConverter::ToChar(bytes, 0);
```

##### `static double ToDouble(Array<byte>& value, int startIndex)`

Returns a double-precision floating-point number converted from eight bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A double-precision floating-point number formed by eight bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(1.234);
double result = BitConverter::ToDouble(bytes, 0);
```

##### `static short ToInt16(Array<byte>& value, int startIndex)`

Returns a 16-bit signed integer converted from two bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 16-bit signed integer formed by two bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes((short)100);
short result = BitConverter::ToInt16(bytes, 0);
```

##### `static int ToInt32(Array<byte>& value, int startIndex)`

Returns a 32-bit signed integer converted from four bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 32-bit signed integer formed by four bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456);
int result = BitConverter::ToInt32(bytes, 0);
```

##### `static long long ToInt64(Array<byte>& value, int startIndex)`

Returns a 64-bit signed integer converted from eight bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 64-bit signed integer formed by eight bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(1234567890123LL);
long long result = BitConverter::ToInt64(bytes, 0);
```

##### `static float ToSingle(Array<byte>& value, int startIndex)`

Returns a single-precision floating-point number converted from four bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A single-precision floating-point number formed by four bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(3.14f);
float result = BitConverter::ToSingle(bytes, 0);
```

##### `static unsigned short ToUInt16(Array<byte>& value, int startIndex)`

Returns a 16-bit unsigned integer converted from two bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 16-bit unsigned integer formed by two bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes((unsigned short)100);
unsigned short result = BitConverter::ToUInt16(bytes, 0);
```

##### `static unsigned int ToUInt32(Array<byte>& value, int startIndex)`

Returns a 32-bit unsigned integer converted from four bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 32-bit unsigned integer formed by four bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(123456U);
unsigned int result = BitConverter::ToUInt32(bytes, 0);
```

##### `static unsigned long long ToUInt64(Array<byte>& value, int startIndex)`

Returns a 64-bit unsigned integer converted from eight bytes at a specified position in a byte array.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A 64-bit unsigned integer formed by eight bytes beginning at `startIndex`.

**Usage:**
```cpp
Array<byte> bytes = BitConverter::GetBytes(1234567890123ULL);
unsigned long long result = BitConverter::ToUInt64(bytes, 0);
```

##### `static String ToString(Array<byte>& value)`

Converts the numeric value of each element of a specified array of bytes to its equivalent hexadecimal string representation.

**Parameters:**
- `value`: An array of bytes.

**Returns:**
- A string of hexadecimal pairs separated by hyphens, where each pair represents the corresponding element in value; for example, "7F-2C-4A".

**Usage:**
```cpp
Array<byte> bytes = { 0x01, 0x02, 0xFF };
String hex = BitConverter::ToString(bytes); // "01-02-FF"
```

##### `static String ToString(Array<byte>& value, int startIndex)`

Converts the numeric value of each element of a specified subarray of bytes to its equivalent hexadecimal string representation.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.

**Returns:**
- A string of hexadecimal pairs separated by hyphens.

**Usage:**
```cpp
Array<byte> bytes = { 0x01, 0x02, 0xFF };
String hex = BitConverter::ToString(bytes, 1); // "02-FF"
```

##### `static String ToString(Array<byte>& value, int startIndex, int length)`

Converts the numeric value of each element of a specified range of bytes within a byte array to its equivalent hexadecimal string representation.

**Parameters:**
- `value`: An array of bytes.
- `startIndex`: The starting position within value.
- `length`: The number of elements to convert.

**Returns:**
- A string of hexadecimal pairs separated by hyphens.

**Usage:**
```cpp
Array<byte> bytes = { 0x01, 0x02, 0xFF, 0xAA };
String hex = BitConverter::ToString(bytes, 1, 2); // "02-FF"
```

##### `static long long DoubleToInt64Bits(double value)`

Converts the specified double-precision floating-point number to a 64-bit signed integer.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- A 64-bit signed integer whose value is equivalent to value.

**Usage:**
```cpp
long long bits = BitConverter::DoubleToInt64Bits(3.14);
```

##### `static double Int64BitsToDouble(long long value)`

Converts the specified 64-bit signed integer to a double-precision floating-point number.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- A double-precision floating-point number whose value is equivalent to value.

**Usage:**
```cpp
double val = BitConverter::Int64BitsToDouble(bits);
```

##### `static int SingleToInt32Bits(float value)`

Converts the specified single-precision floating-point number to a 32-bit signed integer.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- A 32-bit signed integer whose value is equivalent to value.

**Usage:**
```cpp
int bits = BitConverter::SingleToInt32Bits(3.14f);
```

##### `static float Int32BitsToSingle(int value)`

Converts the specified 32-bit signed integer to a single-precision floating-point number.

**Parameters:**
- `value`: The number to convert.

**Returns:**
- A single-precision floating-point number whose value is equivalent to value.

**Usage:**
```cpp
float val = BitConverter::Int32BitsToSingle(bits);
```
