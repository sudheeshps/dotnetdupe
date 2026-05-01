### class `Stream`

Provides a generic view of a sequence of bytes. This is an abstract class.

#### Methods

##### `virtual bool CanRead() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports reading.

**Returns:**
- `true` if the stream supports reading; otherwise, `false`.

**Usage:**
```cpp
if (stream->CanRead()) { /* ... */ }
```

##### `virtual bool CanSeek() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports seeking.

**Returns:**
- `true` if the stream supports seeking; otherwise, `false`.

**Usage:**
```cpp
if (stream->CanSeek()) { /* ... */ }
```

##### `virtual bool CanWrite() const = 0`

When overridden in a derived class, gets a value indicating whether the current stream supports writing.

**Returns:**
- `true` if the stream supports writing; otherwise, `false`.

**Usage:**
```cpp
if (stream->CanWrite()) { /* ... */ }
```

##### `virtual long GetLength() const = 0`

When overridden in a derived class, gets the length in bytes of the stream.

**Returns:**
- A long value representing the length of the stream in bytes.

**Usage:**
```cpp
long len = stream->GetLength();
```

##### `virtual long GetPosition() const = 0`

When overridden in a derived class, gets the position within the current stream.

**Returns:**
- The current position within the stream.

**Usage:**
```cpp
long pos = stream->GetPosition();
```

##### `virtual void SetPosition(long value) = 0`

When overridden in a derived class, sets the position within the current stream.

**Parameters:**
- `value`: The new position within the stream.

**Usage:**
```cpp
stream->SetPosition(0);
```

##### `virtual void Flush() = 0`

When overridden in a derived class, clears all buffers for this stream and causes any buffered data to be written to the underlying device.

**Usage:**
```cpp
stream->Flush();
```

##### `virtual int Read(char* buffer, int offset, int count) = 0`

When overridden in a derived class, reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.

**Parameters:**
- `buffer`: An array of bytes. When this method returns, the buffer contains the specified byte array with the values between `offset` and (`offset` + `count` - 1) replaced by the bytes read from the current source.
- `offset`: The zero-based byte offset in `buffer` at which to begin storing the data read from the current stream.
- `count`: The maximum number of bytes to be read from the current stream.

**Returns:**
- The total number of bytes read into the buffer. This can be less than the number of bytes requested if that many bytes are not currently available, or zero (0) if the end of the stream has been reached.

**Usage:**
```cpp
char buffer[1024];
int read = stream->Read(buffer, 0, 1024);
```

##### `virtual long Seek(long offset, int origin) = 0`

When overridden in a derived class, sets the position within the current stream.

**Parameters:**
- `offset`: A byte offset relative to the `origin` parameter.
- `origin`: A value indicating the reference point used to obtain the new position.

**Returns:**
- The new position within the current stream.

**Usage:**
```cpp
stream->Seek(0, 0);
```

##### `virtual void SetLength(long value) = 0`

When overridden in a derived class, sets the length of the current stream.

**Parameters:**
- `value`: The desired length of the current stream in bytes.

**Usage:**
```cpp
stream->SetLength(1024);
```

##### `virtual void Write(const char* buffer, int offset, int count) = 0`

When overridden in a derived class, writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.

**Parameters:**
- `buffer`: An array of bytes. This method copies `count` bytes from `buffer` to the current stream.
- `offset`: The zero-based byte offset in `buffer` at which to begin copying bytes to the current stream.
- `count`: The number of bytes to be written to the current stream.

**Usage:**
```cpp
const char* data = "Hello";
stream->Write(data, 0, 5);
```

##### `virtual void Dispose() = 0`

Releases all resources used by the `Stream`.

**Usage:**
```cpp
stream->Dispose();
```
