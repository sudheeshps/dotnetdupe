### class `Stream`

Provides a generic view of a sequence of bytes. This is an abstract class.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/IO/Stream.h"
#include "../DotNetDupe/IO/FileStream.h"

int main() {
    // Stream is an abstract class. The following is an example of using a derived class FileStream.
    const DotNetDupe::System::String testFile(_T("test_stream.txt"));
    DotNetDupe::System::IO::FileStream fs(testFile, 0); // FileMode::CreateNew
    char data[] = "Hello Stream!";
    fs.Write(data, 0, sizeof(data));
    fs.Dispose();
    return 0;
}
```

#### Methods

##### `virtual ~Stream() = default`

Destructor for the `Stream` class.

##### `virtual bool CanRead() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports reading.

##### `virtual bool CanSeek() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports seeking.

##### `virtual bool CanWrite() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports writing.

##### `virtual long GetLength() const = 0`

When overridden in a derived class, gets the length in bytes of the stream.

##### `virtual long GetPosition() const = 0`

When overridden in a derived class, gets the position within the current stream.

##### `virtual void SetPosition(long value) = 0`

When overridden in a derived class, sets the position within the current stream.

##### `virtual void Flush() = 0`

When overridden in a derived class, clears all buffers for this stream and causes any buffered data to be written to the underlying device.

##### `virtual int Read(char* buffer, int offset, int count) = 0`

When overridden in a derived class, reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.

##### `virtual long Seek(long offset, int origin) = 0`

When overridden in a derived class, sets the position within the current stream.

##### `virtual void SetLength(long value) = 0`

When overridden in a derived class, sets the length of the current stream.

##### `virtual void Write(const char* buffer, int offset, int count) = 0`

When overridden in a derived class, writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.

##### `virtual void Dispose() = 0`

Releases the unmanaged resources used by the `Stream` and optionally releases the managed resources.
