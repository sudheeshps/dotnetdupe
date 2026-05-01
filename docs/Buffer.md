### class `Buffer`

Manipulates arrays of primitive types.

#### Methods

##### `template <typename T> static void BlockCopy(Array<T>& src, int srcOffset, Array<T>& dst, int dstOffset, int count)`

Copies a specified number of bytes from a source array starting at a particular offset to a destination array starting at a particular offset.

**Parameters:**
- `src`: The source buffer.
- `srcOffset`: The zero-based byte offset into `src`.
- `dst`: The destination buffer.
- `dstOffset`: The zero-based byte offset into `dst`.
- `count`: The number of bytes to copy.

**Usage:**
```cpp
Array<int> src = { 1, 2, 3, 4, 5 };
Array<int> dst(5);
Buffer::BlockCopy(src, 0, dst, 0, 5 * sizeof(int));
```

##### `template <typename T> static int ByteLength(Array<T>& array)`

Returns the number of bytes in the specified array.

**Parameters:**
- `array`: An array.

**Returns:**
- The number of bytes in the array.

**Usage:**
```cpp
Array<int> arr(10);
int bytes = Buffer::ByteLength(arr); // 10 * sizeof(int)
```

##### `template <typename T> static byte GetByte(Array<T>& array, int index)`

Retrieves the byte at a specified location in a specified array.

**Parameters:**
- `array`: An array.
- `index`: A location in the array.

**Returns:**
- The byte at the specified location in the array.

**Usage:**
```cpp
Array<int> arr = { 0x12345678 };
byte b = Buffer::GetByte(arr, 0);
```

##### `template <typename T> static void SetByte(Array<T>& array, int index, byte value)`

Sets a byte at a specified location in a specified array.

**Parameters:**
- `array`: An array.
- `index`: A location in the array.
- `value`: A byte value to set.

**Usage:**
```cpp
Array<int> arr(1);
Buffer::SetByte(arr, 0, 0xAA);
```
