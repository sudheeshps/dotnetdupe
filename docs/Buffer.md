# Buffer

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Buffer.h"`

Manipulates arrays of primitive types at the byte level with bounds-checking.

---

## Syntax

```cpp
class Buffer : public Object;
```

---

## Static Methods

### `template <typename T> static void BlockCopy(Array<T>& src, int srcOffset, Array<T>& dst, int dstOffset, int count)`
Copies a specified number of bytes from a source array starting at a particular offset to a destination array starting at a particular offset.

- **Parameters:**
  - `src` (`Array<T>&`): The source buffer.
  - `srcOffset` (`int`): The zero-based byte offset into `src`.
  - `dst` (`Array<T>&`): The destination buffer.
  - `dstOffset` (`int`): The zero-based byte offset into `dst`.
  - `count` (`int`): The number of bytes to copy.
- **Throws:**
  - `ArgumentNullException`: If `src` or `dst` is empty/null.
  - `ArgumentOutOfRangeException`: If offsets or count are negative.
  - `ArgumentException`: If copying exceeds array byte capacity.

```cpp
Array<int> arrSrc = { 10, 20, 30 };
Array<int> arrDst(3);
Buffer::BlockCopy(arrSrc, 0, arrDst, 0, 3 * sizeof(int));
```

---

### `template <typename T> static int ByteLength(Array<T>& array)`
Returns the total number of bytes in the specified array.

- **Parameters:**
  - `array` (`Array<T>&`): An array of primitive types.
- **Returns:**
  - `int`: The number of bytes in the array (`GetLength() * sizeof(T)`).
- **Throws:**
  - `ArgumentNullException`: If `array` is empty/null.

```cpp
Array<int> arrNumbers(5);
int iBytes = Buffer::ByteLength(arrNumbers); // 20 bytes
```

---

### `template <typename T> static byte GetByte(Array<T>& array, int index)`
Retrieves the byte at a specified location in the specified array.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `index < 0` or `index >= ByteLength(array)`.

---

### `template <typename T> static void SetByte(Array<T>& array, int index, byte value)`
Assigns a specified value to a byte at a particular location in a specified array.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `index < 0` or `index >= ByteLength(array)`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Buffer.h"
#include "System/Array.h"

using namespace DotNetDupe::System;

int main() {
    Array<short> arrSrc = { 100, 200, 300 };
    Array<short> arrDst(3);

    Buffer::BlockCopy(arrSrc, 0, arrDst, 0, Buffer::ByteLength(arrSrc));

    for (int iIdx = 0; iIdx < arrDst.GetLength(); ++iIdx) {
        Console::WriteLine("dst[{0}] = {1}", iIdx, arrDst[iIdx]);
    }

    return 0;
}
```
