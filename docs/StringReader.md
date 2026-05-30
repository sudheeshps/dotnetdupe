### class `StringReader`

Implements a `TextReader` that reads from a string.

#### Methods

##### `StringReader(const String& sInput)`

Initializes a new instance of the `StringReader` class that reads from the specified string.

**Usage:**
```cpp
StringReader srReader("Some text");
```

##### `int Read() override`

Reads the next character from the input string and advances the character position by one character.

**Usage:**
```cpp
int nCh = srReader.Read();
```

##### `String ReadLine() override`

Reads a line of characters from the current string and returns the data as a string.

**Usage:**
```cpp
String sLine = srReader.ReadLine();
```

