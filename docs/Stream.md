# Stream &amp; MemoryStream

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/Stream.h"`, `#include "System/IO/MemoryStream.h"`

`Stream` provides a generic view of a sequence of bytes. `MemoryStream` creates a stream whose backing store is contiguous memory.

---

## `Stream` (Abstract Base Class)

### Syntax
```cpp
class Stream : public Object, public IDisposable;
```

### Pure Virtual Members
- `virtual bool CanRead() const = 0`: Gets a value indicating whether the current stream supports reading.
- `virtual bool CanSeek() const = 0`: Gets a value indicating whether the current stream supports seeking.
- `virtual bool CanWrite() const = 0`: Gets a value indicating whether the current stream supports writing.
- `virtual long GetLength() const = 0`: Gets the length in bytes of the stream.
- `virtual long GetPosition() const = 0` / `virtual void SetPosition(long value) = 0`: Gets or sets the position within the current stream.
- `virtual void Flush() = 0`: Clears all buffers for this stream and causes any buffered data to be written to the underlying device.
- `virtual int Read(char* buffer, int offset, int count) = 0`: Reads a sequence of bytes from the current stream and advances the position.
- `virtual void Write(const char* buffer, int offset, int count) = 0`: Writes a sequence of bytes to the current stream and advances the current position.
- `virtual long Seek(long offset, int origin) = 0`: Sets the position within the current stream (0 = Begin, 1 = Current, 2 = End).
- `virtual void SetLength(long value) = 0`: Sets the length of the current stream.
- `virtual void Dispose() = 0`: Releases all unmanaged resources used by the `Stream`.

---

## `MemoryStream`

### Syntax
```cpp
class MemoryStream : public Stream;
```

### Constructors
- `MemoryStream()`: Initializes a new non-resizable instance with an expandable capacity initialized to 0.
- `explicit MemoryStream(const Array<char>& buffer)`: Initializes a new non-resizable instance based on the specified byte array.
- `MemoryStream(const Array<char>& buffer, bool writable)`: Initializes a new instance with optional write permissions.

### Additional Methods
- `Array<char> ToArray() const`: Writes the entire stream contents to a byte array, regardless of the `Position` property.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/MemoryStream.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    MemoryStream memStream;

    const char* text = "Hello from DotNetDupe MemoryStream!";
    int len = (int)strlen(text);
    memStream.Write(text, 0, len);

    Console::WriteLine("Stream Length: {0} bytes", memStream.GetLength());
    Console::WriteLine("Position:      {0}", memStream.GetPosition());

    // Reset position to read
    memStream.SetPosition(0);

    char readBuf[128] = { 0 };
    int bytesRead = memStream.Read(readBuf, 0, sizeof(readBuf) - 1);

    Console::WriteLine("Bytes Read: {0}, Content: {1}", bytesRead, String(readBuf));

    return 0;
}
```
