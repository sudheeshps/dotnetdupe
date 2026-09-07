# BinaryWriter

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/BinaryWriter.h"`

`BinaryWriter` writes primitive data types in binary format to an output stream. It supports configurable Little-Endian and Big-Endian byte ordering, buffer slicing, seeking within seekable streams, and automatic or manual stream disposal.

---

## Syntax

```cpp
namespace DotNetDupe {
    namespace System {
        namespace IO {
            class BinaryWriter : public Object, public IDisposable;
        }
    }
}
```

---

## Constructors

### `BinaryWriter(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)`
Initializes a new instance of the `BinaryWriter` class based on the specified stream.
- **`pStream`**: Non-owning pointer to the backing `Stream`. Throws `ArgumentNullException` if `nullptr`.
- **`bLeaveOpen`**: If `true`, the underlying stream is left open after the `BinaryWriter` object is disposed or closed; otherwise `false`.
- **`bIsLittleEndian`**: If `true`, values are serialized in Little-Endian format; if `false`, Big-Endian format is used. Defaults to `true`.

### `BinaryWriter(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)`
Initializes a new instance of the `BinaryWriter` class with shared ownership of the backing stream.
- **`spStream`**: Smart pointer to the backing `Stream`. Throws `ArgumentNullException` if empty.
- **`bLeaveOpen`**: Controls stream disposal upon writer cleanup.
- **`bIsLittleEndian`**: Byte order configuration.

---

## Configuration & Stream Navigation

- `Stream* GetBaseStream() const`: Returns a pointer to the underlying stream.
- `bool IsLittleEndian() const`: Returns `true` if configured for Little-Endian serialization; otherwise `false`.
- `void SetLittleEndian(bool bIsLittleEndian)`: Dynamically changes the byte ordering for subsequent write operations.
- `long Seek(long lOffset, int iOrigin)`: Sets the position within the current stream (if supported by the underlying stream).
- `void Flush()`: Clears all buffers for the current writer and causes any buffered data to be written to the underlying stream.

---

## Write Methods

| Method | Description |
|---|---|
| `Write(bool bValue)` | Writes a 1-byte `bool` value (`1` for `true`, `0` for `false`). |
| `Write(byte bValue)` | Writes an unsigned byte to the current stream and advances the stream position. |
| `Write(signed char chValue)` | Writes a signed byte to the current stream. |
| `Write(const Array<byte>& arrBuffer)` | Writes the entire byte array to the stream. |
| `Write(const Array<byte>& arrBuffer, int iIndex, int iCount)` | Writes a subsection of `iCount` bytes from `arrBuffer` starting at `iIndex`. |
| `Write(short iValue)` | Writes a 2-byte signed integer using configured endianness. |
| `Write(unsigned short uValue)` | Writes a 2-byte unsigned integer using configured endianness. |
| `Write(int iValue)` | Writes a 4-byte signed integer using configured endianness. |
| `Write(unsigned int uValue)` | Writes a 4-byte unsigned integer using configured endianness. |
| `Write(long long llValue)` | Writes an 8-byte signed integer using configured endianness. |
| `Write(unsigned long long ullValue)` | Writes an 8-byte unsigned integer using configured endianness. |
| `Write(float fValue)` | Writes a 4-byte floating-point value to the current stream. |
| `Write(double dValue)` | Writes an 8-byte floating-point value to the current stream. |
| `Write(const String& sValue)` | Writes an ASCII/UTF-8 string directly as raw bytes without length prefix. |

---

## Lifecycle & Disposal

- `virtual void Close()`: Flushes and closes the current writer and the underlying stream (if `bLeaveOpen` is `false`).
- `virtual void Dispose() override`: Releases all resources used by the `BinaryWriter`.

---

## Exceptions

- `DotNetDupe::System::ArgumentNullException`: Thrown when a null stream pointer is passed to the constructor.
- `DotNetDupe::System::ArgumentOutOfRangeException`: Thrown when negative counts, invalid indices, or invalid seek parameters are provided.
- `DotNetDupe::System::InvalidOperationException`: Thrown when attempting write operations on a disposed writer.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/BinaryWriter.h"
#include "System/IO/FileStream.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

void WriteHeaderData(const String& filePath) {
    FileStream stream(filePath, FileMode::Create, FileAccess::Write);
    
    // Write in Little-Endian
    BinaryWriter writer(&stream, true /* leaveOpen */, true /* littleEndian */);
    writer.Write(static_cast<byte>(0x01));      // Version
    writer.Write(42);                           // ID
    writer.Write(199.95);                       // Price
    writer.Write(String("PRODUCT_CODE_1"));     // Payload
    writer.Flush();
}
```
