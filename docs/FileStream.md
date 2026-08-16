# FileStream

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/FileStream.h"`

Provides a `Stream` for a file, supporting both synchronous read and write operations, seeking, and deterministic resource release via `IDisposable`.

---

## Syntax

```cpp
class FileStream : public Stream;
```

---

## Constructors

### `FileStream(const String& sPath, int iMode)`
Initializes a new instance of the `FileStream` class with the specified path and creation mode.
- **Parameters:**
  - `sPath` (`const String&`): A relative or absolute path for the file that the current `FileStream` object will encapsulate.
  - `iMode` (`int`): Constant specifying how the operating system should open the file (Create, Open, Append, etc.).
- **Throws:**
  - `IOException`: If the file cannot be opened.

---

## Member Functions

### `bool CanRead() const override`
Gets a value indicating whether the current stream supports reading.

### `bool CanSeek() const override`
Gets a value indicating whether the current stream supports seeking.

### `bool CanWrite() const override`
Gets a value indicating whether the current stream supports writing.

### `long GetLength() const override`
Gets the length in bytes of the stream.

### `long GetPosition() const override` / `void SetPosition(long llValue) override`
Gets or sets the current position of this stream.

### `int Read(char* pBuffer, int iOffset, int nCount) override`
Reads a block of bytes from the stream and writes the data in a given buffer.
- **Returns:**
  - `int`: The total number of bytes read into the buffer, or `0` if the end of the stream is reached.

### `void Write(const char* pBuffer, int iOffset, int nCount) override`
Writes a block of bytes to the file stream.

### `long Seek(long llOffset, int iOrigin) override`
Sets the current position of this stream to the given value (0 = Begin, 1 = Current, 2 = End).

### `void Flush() override`
Clears buffers for this stream and causes any unwritten data to be written to the file.

### `void Dispose() override`
Closes the file handle and releases all unmanaged operating system resources.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/FileStream.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String filePath = "binary.dat";

    // Write data
    {
        auto pStream = SmartPointer<FileStream>::NewUnique(filePath, 2 /* Create */);
        const char* msg = "Native Binary Data";
        pStream->Write(msg, 0, (int)strlen(msg));
    } // FileStream automatically disposed and flushed

    // Read data
    {
        auto pStream = SmartPointer<FileStream>::NewUnique(filePath, 3 /* Open */);
        char buf[64] = { 0 };
        int bytes = pStream->Read(buf, 0, sizeof(buf) - 1);
        Console::WriteLine("Read {0} bytes: {1}", bytes, String(buf));
    }

    return 0;
}
```
