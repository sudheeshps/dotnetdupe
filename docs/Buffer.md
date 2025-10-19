### class `Buffer`

Manipulates arrays of primitive types.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Buffer.h"
#include "../DotNetDupe/Array.h"

int main() {
    DotNetDupe::System::Array<byte> src(5);
    src[0] = 1;
    src[1] = 2;
    src[2] = 3;
    src[3] = 4;
    src[4] = 5;
    DotNetDupe::System::Array<byte> dst(5);
    DotNetDupe::System::Buffer::BlockCopy(src, 0, dst, 0, 5);
    for (int i = 0; i < dst.GetLength(); i++) {
        std::cout << (int)dst[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

#### Methods

##### `template <typename T> static void BlockCopy(Array<T>& src, int srcOffset, Array<T>& dst, int dstOffset, int count)`

Copies a specified number of bytes from a source array starting at a particular offset to a destination array starting at a particular offset.

##### `template <typename T> static int ByteLength(Array<T>& array)`

Returns the number of bytes in the specified array.

##### `template <typename T> static byte GetByte(Array<T>& array, int index)`

Retrieves the byte at a specified location in a specified array.

##### `template <typename T> static void SetByte(Array<T>& array, int index, byte value)`

Sets a byte at a specified location in a specified array.
