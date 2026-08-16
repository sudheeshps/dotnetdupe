# TextReader &amp; StringReader

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/TextReader.h"`, `#include "System/IO/StringReader.h"`

`TextReader` represents an abstract reader that can read a sequential series of characters. `StringReader` implements `TextReader` to read characters sequentially from a `String`.

---

## `TextReader` (Abstract Base Class)

### Syntax
```cpp
class TextReader : public Object, public IDisposable;
```

### Pure Virtual Members
- `virtual int Read() = 0`: Reads the next character from the text reader and advances the character position by one character. Returns `-1` if no characters remain.
- `virtual int Read(char* pBuffer, int iIndex, int nCount) = 0`: Reads a specified maximum number of characters from the current reader and writes the data to a buffer.
- `virtual int Peek() = 0`: Reads the next character without advancing the character position.
- `virtual String ReadLine() = 0`: Reads a line of characters from the text reader and returns the data as a `String`.
- `virtual String ReadToEnd() = 0`: Reads all characters from the current position to the end of the text reader and returns them as one string.
- `virtual void Close() = 0`: Closes the `TextReader` and releases any system resources.
- `virtual void Dispose() override = 0`: Releases all resources used by the `TextReader` object.

---

## `StringReader`

### Syntax
```cpp
class StringReader : public TextReader;
```

### Constructors
- `StringReader(const String& sSource)`: Initializes a new instance of the `StringReader` class that reads from the specified string.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/StringReader.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String text = "First Line\nSecond Line\nThird Line";
    StringReader reader(text);

    String line;
    while ((line = reader.ReadLine()).GetLength() > 0) {
        Console::WriteLine("Read Line: {0}", line);
    }

    return 0;
}
```
