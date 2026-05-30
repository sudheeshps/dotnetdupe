### class `TextWriter` (Abstract)

Represents a writer that can write a sequential series of characters. This is an **abstract base class**; use a derived class like `StringWriter` for actual writing operations.

#### Methods

##### `virtual void Close() = 0`

Closes the current writer and releases any system resources associated with the writer.

**Usage:**
```cpp
pWriter->Close();
```

##### `virtual void Dispose() = 0`

Releases all resources used by the `TextWriter` object.

**Usage:**
```cpp
pWriter->Dispose();
```

##### `virtual void Flush() = 0`

Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.

**Usage:**
```cpp
pWriter->Flush();
```

##### `virtual std::shared_ptr<Text::Encoding> GetEncoding() const = 0`

When overridden in a derived class, returns the character encoding in which the output is written.

##### `virtual void Write(const String& sValue) = 0`

Writes a string to the text stream.

**Usage:**
```cpp
pWriter->Write("Hello");
```

##### `virtual void Write(char cValue) = 0`
##### `virtual void Write(bool bValue) = 0`
##### `virtual void Write(int nValue) = 0`
##### `virtual void Write(double dValue) = 0`

Overloaded methods to write various primitive types to the text stream.

##### `virtual void WriteLine(const String& sValue) = 0`

Writes a string followed by a line terminator to the text stream.

**Usage:**
```cpp
pWriter->WriteLine("Hello World");
```

##### `virtual void WriteLine()` = 0

Writes a line terminator to the text stream.

**Usage:**
```cpp
pWriter->WriteLine();
```

##### `virtual void WriteLine(char cValue) = 0`
##### `virtual void WriteLine(bool bValue) = 0`
##### `virtual void WriteLine(int nValue) = 0`
##### `virtual void WriteLine(double dValue) = 0`

Overloaded methods to write various primitive types followed by a line terminator to the text stream.

