### class `TextReader` (Abstract)

Represents a reader that can read a sequential series of characters. This is an **abstract base class**; use a derived class like `StringReader` for actual reading operations.

#### Methods

##### `virtual void Close() = 0`

Closes the `TextReader` and releases any system resources associated with the reader.

**Usage:**
```cpp
pReader->Close();
```

##### `virtual void Dispose() = 0`

Releases all resources used by the `TextReader` object.

**Usage:**
```cpp
pReader->Dispose();
```

##### `virtual int Peek() = 0`

Reads the next character without changing the state of the reader or the character source. Returns -1 if no more characters are available.

**Usage:**
```cpp
int nNextChar = pReader->Peek();
```

##### `virtual int Read() = 0`

Reads the next character from the text reader and advances the character position by one character. Returns -1 if no more characters are available.

**Usage:**
```cpp
int nCh = pReader->Read();
```

##### `virtual int Read(char* pBuffer, int nIndex, int nCount) = 0`

Reads a specified maximum number of characters from the current reader and writes the data to a buffer, beginning at the specified nIndex.

**Usage:**
```cpp
char pBuffer[100];
int nBytesRead = pReader->Read(pBuffer, 0, 100);
```

##### `virtual String ReadLine() = 0`

Reads a line of characters from the text reader and returns the data as a string. Returns an empty string if the end of the input stream is reached.

**Usage:**
```cpp
String sLine = pReader->ReadLine();
```

##### `virtual String ReadToEnd() = 0`

Reads all characters from the current position to the end of the text reader and returns them as one string.

**Usage:**
```cpp
String sContent = pReader->ReadToEnd();
```

