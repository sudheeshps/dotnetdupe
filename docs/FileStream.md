### class `FileStream`

Provides a `Stream` for a file, supporting synchronous read and write operations.

#### Methods

##### `FileStream(const String& path, int mode)`

Initializes a new instance of the `FileStream` class with the specified path and creation mode.

**Parameters:**
- `path`: A relative or absolute path for the file that the current `FileStream` object will encapsulate.
- `mode`: A constant that determines how to open or create the file.

**Usage:**
```cpp
FileStream fs(_T("test.dat"), 1); // OpenOrCreate
```

##### `bool CanRead() const override`

Gets a value indicating whether the current stream supports reading.

**Returns:**
- `true` if the stream supports reading; otherwise, `false`.

**Usage:**
```cpp
if (fs.CanRead()) { /* ... */ }
```

##### `bool CanSeek() const override`

Gets a value indicating whether the current stream supports seeking.

**Returns:**
- `true` if the stream supports seeking; otherwise, `false`.

**Usage:**
```cpp
if (fs.CanSeek()) { /* ... */ }
```

##### `bool CanWrite() const override`

Gets a value indicating whether the current stream supports writing.

**Returns:**
- `true` if the stream supports writing; otherwise, `false`.

**Usage:**
```cpp
if (fs.CanWrite()) { /* ... */ }
```

##### `long GetLength() const override`

Gets the length in bytes of the stream.

**Returns:**
- A long value representing the length of the stream in bytes.

**Usage:**
```cpp
long len = fs.GetLength();
```

##### `long GetPosition() const override`

Gets the current position of this stream.

**Returns:**
- The current position within the stream.

**Usage:**
```cpp
long pos = fs.GetPosition();
```

##### `void SetPosition(long value) override`

Sets the current position of this stream.

**Parameters:**
- `value`: The new position within the stream.

**Usage:**
```cpp
fs.SetPosition(100);
```

##### `void Flush() override`

Clears all buffers for this stream and causes any buffered data to be written to the underlying device.

**Usage:**
```cpp
fs.Flush();
```

##### `int Read(char* buffer, int offset, int count) override`

Reads a block of bytes from the stream and writes the data in a given buffer.

**Parameters:**
- `buffer`: When this method returns, contains the specified character array with the values between `offset` and (`offset` + `count` - 1) replaced by the characters read from the current source.
- `offset`: The zero-based byte offset in `buffer` at which to begin storing the data read from the current stream.
- `count`: The maximum number of bytes to be read from the current stream.

**Returns:**
- The total number of bytes read into the buffer. This can be less than the number of bytes requested if that many bytes are not currently available, or zero (0) if the end of the stream has been reached.

**Usage:**
```cpp
char buffer[1024];
int read = fs.Read(buffer, 0, 1024);
```

##### `long Seek(long offset, int origin) override`

Sets the position within the current stream.

**Parameters:**
- `offset`: A byte offset relative to the `origin` parameter.
- `origin`: A value indicating the reference point used to obtain the new position.

**Returns:**
- The new position within the current stream.

**Usage:**
```cpp
fs.Seek(0, 0); // Seek to beginning
```

##### `void SetLength(long value) override`

Sets the length of this stream to the given value.

**Parameters:**
- `value`: The desired length of the current stream in bytes.

**Usage:**
```cpp
fs.SetLength(2048);
```

##### `void Write(const char* buffer, int offset, int count) override`

Writes a block of bytes to this stream using data from a buffer.

**Parameters:**
- `buffer`: The buffer containing data to write to the stream.
- `offset`: The zero-based byte offset in `buffer` at which to begin copying bytes to the current stream.
- `count`: The number of bytes to be written to the current stream.

**Usage:**
```cpp
const char* data = "Hello";
fs.Write(data, 0, 5);
```

##### `void Dispose() override`

Releases all resources used by the `FileStream`.

**Usage:**
```cpp
fs.Dispose();
```
