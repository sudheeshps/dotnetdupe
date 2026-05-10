### class `TextWriter`

Represents a writer that can write a sequential series of characters.

#### Methods

##### `virtual void Close()`

Closes the current writer and releases any system resources associated with the writer.

**Usage:**
```cpp
writer->Close();
```

##### `virtual void Flush()`

Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.

**Usage:**
```cpp
writer->Flush();
```

##### `virtual void Write(const String& value)`

Writes a string to the text stream.

**Usage:**
```cpp
writer->Write(_T("Hello"));
```

##### `virtual void WriteLine(const String& value)`

Writes a string followed by a line terminator to the text stream.

**Usage:**
```cpp
writer->WriteLine(_T("Hello World"));
```

##### `virtual void WriteLine()`

Writes a line terminator to the text stream.

**Usage:**
```cpp
writer->WriteLine();
```
