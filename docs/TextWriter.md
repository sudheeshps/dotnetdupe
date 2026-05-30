### class `TextWriter` (Abstract)

Represents a writer that can write a sequential series of characters. This is an **abstract base class**; use a derived class like `StringWriter` for actual writing operations.

#### Methods

##### `virtual void Close() = 0`

Closes the current writer and releases any system resources associated with the writer.

**Usage:**
```cpp
writer->Close();
```

##### `virtual void Dispose() = 0`

Releases all resources used by the `TextWriter` object.

**Usage:**
```cpp
writer->Dispose();
```

##### `virtual void Flush() = 0`

Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.

**Usage:**
```cpp
writer->Flush();
```

##### `virtual std::shared_ptr<Text::Encoding> GetEncoding() const = 0`

When overridden in a derived class, returns the character encoding in which the output is written.

##### `virtual void Write(const String& value) = 0`

Writes a string to the text stream.

**Usage:**
```cpp
writer->Write("Hello");
```

##### `virtual void Write(char value) = 0`
##### `virtual void Write(bool value) = 0`
##### `virtual void Write(int value) = 0`
##### `virtual void Write(double value) = 0`

Overloaded methods to write various primitive types to the text stream.

##### `virtual void WriteLine(const String& value) = 0`

Writes a string followed by a line terminator to the text stream.

**Usage:**
```cpp
writer->WriteLine("Hello World");
```

##### `virtual void WriteLine()` = 0

Writes a line terminator to the text stream.

**Usage:**
```cpp
writer->WriteLine();
```

##### `virtual void WriteLine(char value) = 0`
##### `virtual void WriteLine(bool value) = 0`
##### `virtual void WriteLine(int value) = 0`
##### `virtual void WriteLine(double value) = 0`

Overloaded methods to write various primitive types followed by a line terminator to the text stream.
