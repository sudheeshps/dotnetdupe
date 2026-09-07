# BinaryReader

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/BinaryReader.h"`

`BinaryReader` reads primitive data types as binary values in a specific encoding from a stream. It supports both Little-Endian and Big-Endian byte ordering, making it ideal for low-level binary serialization, cross-platform file format parsers (such as DICOM), and network communication.

---

## Syntax

```cpp
namespace DotNetDupe {
    namespace System {
        namespace IO {
            class BinaryReader : public Object, public IDisposable;
        }
    }
}
```

---

## Constructors

### `BinaryReader(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)`
Initializes a new instance of the `BinaryReader` class based on the specified stream.
- **`pStream`**: Non-owning pointer to the backing `Stream`. Throws `ArgumentNullException` if `nullptr`.
- **`bLeaveOpen`**: If `true`, the underlying stream is left open after the `BinaryReader` object is disposed or closed; otherwise `false`.
- **`bIsLittleEndian`**: If `true`, multi-byte integers and floating-point numbers are read using Little-Endian order; if `false`, Big-Endian order is used. Defaults to `true`.

### `BinaryReader(SmartPointer<Stream> spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)`
Initializes a new instance of the `BinaryReader` class with shared ownership of the backing stream.
- **`spStream`**: Smart pointer to the backing `Stream`. Throws `ArgumentNullException` if empty.
- **`bLeaveOpen`**: Controls stream disposal upon reader cleanup.
- **`bIsLittleEndian`**: Byte order configuration.

---

## Configuration & Stream Inspection

- `Stream* GetBaseStream() const`: Exposes access to the underlying stream.
- `bool IsLittleEndian() const`: Returns `true` if configured for Little-Endian reading; otherwise `false`.
- `void SetLittleEndian(bool bIsLittleEndian)`: Dynamically changes the endianness mode for subsequent read operations.

---

## Read Methods

| Method | Return Type | Description |
|---|---|---|
| `ReadBoolean()` | `bool` | Reads a 1-byte `bool` value (`true` for non-zero, `false` for zero). |
| `ReadByte()` | `byte` | Reads the next unsigned byte (`uint8_t`) from the stream. |
| `ReadSByte()` | `signed char` | Reads a signed 1-byte integer from the stream. |
| `ReadChar()` | `char` | Reads the next character from the current stream. |
| `ReadBytes(int iCount)` | `Array<byte>` | Reads the specified number of bytes into a newly allocated `Array<byte>`. |
| `Read(Array<byte>& arrBuffer, int iIndex, int iCount)` | `int` | Reads up to `iCount` bytes from the stream into `arrBuffer` starting at `iIndex`. |
| `ReadInt16()` | `short` | Reads a 2-byte signed integer using configured endianness. |
| `ReadUInt16()` | `unsigned short` | Reads a 2-byte unsigned integer using configured endianness. |
| `ReadInt32()` | `int` | Reads a 4-byte signed integer using configured endianness. |
| `ReadUInt32()` | `unsigned int` | Reads a 4-byte unsigned integer using configured endianness. |
| `ReadInt64()` | `long long` | Reads an 8-byte signed integer using configured endianness. |
| `ReadUInt64()` | `unsigned long long` | Reads an 8-byte unsigned integer using configured endianness. |
| `ReadSingle()` | `float` | Reads a 4-byte single-precision floating-point value. |
| `ReadDouble()` | `double` | Reads an 8-byte double-precision floating-point value. |
| `ReadString(int iLength)` | `String` | Reads a sequence of `iLength` bytes as an ASCII/UTF-8 `String`. |

---

## Lifecycle & Disposal

- `virtual void Close()`: Closes the current reader and the underlying stream (if `bLeaveOpen` is `false`).
- `virtual void Dispose() override`: Releases all resources held by the reader.

---

## Exceptions

- `DotNetDupe::System::ArgumentNullException`: Thrown when a null stream pointer is passed to the constructor.
- `DotNetDupe::System::ArgumentOutOfRangeException`: Thrown when `iCount` or `iIndex` is negative, or invalid buffer bounds are supplied.
- `DotNetDupe::System::IO::EndOfStreamException`: Thrown when reading beyond the available stream length.
- `DotNetDupe::System::InvalidOperationException`: Thrown when attempting an operation on an already disposed reader.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/MemoryStream.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

void ReadBinaryData(Stream& stream) {
    // Read using little-endian byte ordering
    BinaryReader reader(&stream, true /* leaveOpen */, true /* littleEndian */);

    bool bFlag       = reader.ReadBoolean();
    byte bTag        = reader.ReadByte();
    int nRecordId    = reader.ReadInt32();
    float fScore     = reader.ReadSingle();
    String sName     = reader.ReadString(10);

    Console::WriteLine("Tag: {0}, Record: {1}, Score: {2}, Name: {3}",
        bTag, nRecordId, fScore, sName);
}
```
