### class `FileStream`

Provides a `Stream` for a file, supporting both synchronous and asynchronous read and write operations.

#### Usage

```cpp
#include <iostream>
#include <vector>
#include "../DotNetDupe/IO/FileStream.h"
#include "../DotNetDupe/IO/File.h"
#include "../DotNetDupe/Text/TextEncoding.h"

int main() {
    const DotNetDupe::System::String testFile(_T("test_filestream.txt"));
    const DotNetDupe::System::String content(_T("Hello FileStream!"));

    DotNetDupe::System::IO::FileStream fs(testFile, 0); // FileMode::CreateNew
    std::vector<char> contentBytes = DotNetDupe::System::Text::TextEncoding::UTF8()->GetBytes(content);
    fs.Write(contentBytes.data(), 0, static_cast<int>(contentBytes.size()));
    fs.SetPosition(0);
    std::vector<char> readBuffer(contentBytes.size());
    int bytesRead = fs.Read(readBuffer.data(), 0, static_cast<int>(readBuffer.size()));
    DotNetDupe::System::String readContent = DotNetDupe::System::Text::TextEncoding::UTF8()->GetString(readBuffer);
    std::wcout << L"Read from FileStream: " << readContent.GetRawString() << std::endl;
    fs.Dispose();
    DotNetDupe::System::IO::File::Delete(testFile);

    return 0;
}
```

#### Methods

##### `FileStream(const String& path, int mode)`

Initializes a new instance of the `FileStream` class with the specified path and creation mode.

##### `~FileStream()`

Destructor for the `FileStream` class.

##### `bool CanRead() const override`

Gets a value indicating whether the current stream supports reading.

##### `bool CanSeek() const override`

Gets a value indicating whether the current stream supports seeking.

##### `bool CanWrite() const override`

Gets a value indicating whether the current stream supports writing.

##### `long GetLength() const override`

Gets the length in bytes of the stream.

##### `long GetPosition() const override`

Gets the current position of this stream.

##### `void SetPosition(long value) override`

Sets the current position of this stream.

##### `void Flush() override`

Clears all buffers for this stream and causes any buffered data to be written to the underlying device.

##### `int Read(char* buffer, int offset, int count) override`

Reads a block of bytes from the stream and writes the data in a given buffer.

##### `long Seek(long offset, int origin) override`

Sets the position within the current stream.

##### `void SetLength(long value) override`

Sets the length of this stream to the given value.

##### `void Write(const char* buffer, int offset, int count) override`

Writes a block of bytes to this stream using data from a buffer.

##### `void Dispose() override`

Releases the unmanaged resources used by the `FileStream` and optionally releases the managed resources.
