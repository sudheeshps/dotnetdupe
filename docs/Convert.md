### class `Convert`

Provides methods for converting a base data type to another base data type.

#### Methods

##### `static bool ToBoolean(...)`

Converts a specified value to an equivalent Boolean value. Overloads are available for `bool`, `int`, `long long`, `double`, and `String`.

**Usage:**
```cpp
bool b1 = Convert::ToBoolean(1); // true
bool b2 = Convert::ToBoolean("true"); // true
```

##### `static unsigned char ToByte(...)`

Converts a specified value to an 8-bit unsigned integer.

**Usage:**
```cpp
unsigned char b = Convert::ToByte(255);
unsigned char b2 = Convert::ToByte("FF", 16); // 255 from Hex
```

##### `static signed char ToSByte(...)`

Converts a specified value to an 8-bit signed integer.

**Usage:**
```cpp
signed char sb = Convert::ToSByte(127);
```

##### `static TCHAR ToChar(...)`

Converts a specified value to a Unicode character.

**Usage:**
```cpp
TCHAR c = Convert::ToChar(65); // 'A'
TCHAR c2 = Convert::ToChar("A"); // 'A'
```

##### `static double ToDouble(...)`

Converts a specified value to a double-precision floating-point number.

**Usage:**
```cpp
double d = Convert::ToDouble("3.14159");
```

##### `static float ToSingle(...)`

Converts a specified value to a single-precision floating-point number.

**Usage:**
```cpp
float f = Convert::ToSingle("1.23");
```

##### `static short ToInt16(...)`

Converts a specified value to a 16-bit signed integer.

**Usage:**
```cpp
short s = Convert::ToInt16("123");
```

##### `static int ToInt32(...)`

Converts a specified value to a 32-bit signed integer. Overloads support base-specified conversion.

**Usage:**
```cpp
int i = Convert::ToInt32("12345");
int i2 = Convert::ToInt32("FF", 16); // 255
```

##### `static long long ToInt64(...)`

Converts a specified value to a 64-bit signed integer.

**Usage:**
```cpp
long long l = Convert::ToInt64("123456789012345");
```

##### `static unsigned short ToUInt16(...)`

Converts a specified value to a 16-bit unsigned integer.

**Usage:**
```cpp
unsigned short us = Convert::ToUInt16("65535");
```

##### `static unsigned int ToUInt32(...)`

Converts a specified value to a 32-bit unsigned integer.

**Usage:**
```cpp
unsigned int ui = Convert::ToUInt32("4294967295");
```

##### `static unsigned long long ToUInt64(...)`

Converts a specified value to a 64-bit unsigned integer.

**Usage:**
```cpp
unsigned long long ull = Convert::ToUInt64("18446744073709551615");
```

##### `static String ToString(...)`

Converts the specified value to its equivalent String representation. Overloads support base-specified conversion for integers.

**Usage:**
```cpp
String s1 = Convert::ToString(true); // "True"
String s2 = Convert::ToString(255, 16); // "FF"
```

