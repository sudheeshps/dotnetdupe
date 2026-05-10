### class `TextReader`

Represents a reader that can read a sequential series of characters.

#### Methods

##### `virtual void Close()`

Closes the `TextReader` and releases any system resources associated with the reader.

**Usage:**
```cpp
reader->Close();
```

##### `virtual int Peek()`

Reads the next character without changing the state of the reader or the character source. Returns -1 if no more characters are available.

**Usage:**
```cpp
int nextChar = reader->Peek();
```

##### `virtual int Read()`

Reads the next character from the text reader and advances the character position by one character. Returns -1 if no more characters are available.

**Usage:**
```cpp
int ch = reader->Read();
```

##### `virtual int Read(wchar_t* buffer, int index, int count)`

Reads a specified maximum number of characters from the current reader and writes the data to a buffer, beginning at the specified index.

**Usage:**
```cpp
wchar_t buffer[100];
int read = reader->Read(buffer, 0, 100);
```

##### `virtual String ReadLine()`

Reads a line of characters from the text reader and returns the data as a string.

**Usage:**
```cpp
String line = reader->ReadLine();
```

##### `virtual String ReadToEnd()`

Reads all characters from the current position to the end of the text reader and returns them as one string.

**Usage:**
```cpp
String content = reader->ReadToEnd();
```
