### class `TextReader` (Abstract)

Represents a reader that can read a sequential series of characters. This is an **abstract base class**; use a derived class like `StringReader` for actual reading operations.

#### Methods

##### `virtual void Close() = 0`

Closes the `TextReader` and releases any system resources associated with the reader.

**Usage:**
```cpp
reader->Close();
```

##### `virtual void Dispose() = 0`

Releases all resources used by the `TextReader` object.

**Usage:**
```cpp
reader->Dispose();
```

##### `virtual int Peek() = 0`

Reads the next character without changing the state of the reader or the character source. Returns -1 if no more characters are available.

**Usage:**
```cpp
int nextChar = reader->Peek();
```

##### `virtual int Read() = 0`

Reads the next character from the text reader and advances the character position by one character. Returns -1 if no more characters are available.

**Usage:**
```cpp
int ch = reader->Read();
```

##### `virtual int Read(char* buffer, int index, int count) = 0`

Reads a specified maximum number of characters from the current reader and writes the data to a buffer, beginning at the specified index.

**Usage:**
```cpp
char buffer[100];
int bytesRead = reader->Read(buffer, 0, 100);
```

##### `virtual String ReadLine() = 0`

Reads a line of characters from the text reader and returns the data as a string. Returns an empty string if the end of the input stream is reached.

**Usage:**
```cpp
String line = reader->ReadLine();
```

##### `virtual String ReadToEnd() = 0`

Reads all characters from the current position to the end of the text reader and returns them as one string.

**Usage:**
```cpp
String content = reader->ReadToEnd();
```
