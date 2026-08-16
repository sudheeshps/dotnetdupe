# TextWriter &amp; StringWriter

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/TextWriter.h"`, `#include "System/IO/StringWriter.h"`

`TextWriter` represents an abstract writer that can write a sequential series of characters. `StringWriter` implements `TextWriter` for writing information into a string or `StringBuilder`.

---

## `TextWriter` (Abstract Base Class)

### Syntax
```cpp
class TextWriter : public Object, public IDisposable;
```

### Pure Virtual Members
- `virtual void Write(bool / char / const char* / const String& / int / long long / float / double) = 0`: Writes a formatted representation of primitive types to the text stream.
- `virtual void WriteLine() = 0`: Writes a line terminator to the text stream.
- `virtual void WriteLine(bool / char / const char* / const String& / int / long long / float / double) = 0`: Writes data followed by a line terminator to the text stream.
- `virtual void Flush() = 0`: Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.
- `virtual void Close() = 0`: Closes the current `TextWriter` and releases any system resources associated with the writer.
- `virtual void Dispose() override = 0`: Releases all resources used by the `TextWriter` object.
- `virtual Text::EncodingPtr GetEncoding() const = 0`: Returns the character encoding in which the output is written.

---

## `StringWriter`

### Syntax
```cpp
class StringWriter : public TextWriter;
```

### Constructors
- `StringWriter()`: Initializes a new instance of the `StringWriter` class.
- `StringWriter(Text::StringBuilder& sbOutput)`: Initializes a new instance of the `StringWriter` class that writes to the specified `StringBuilder`.

### Additional Methods
- `virtual String ToString() const`: Returns a string containing the characters written to the current `StringWriter` so far.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/StringWriter.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    StringWriter writer;
    writer.WriteLine("Header: Application Report");
    writer.Write("Result Code: ");
    writer.WriteLine(200);
    writer.Write("Status: ");
    writer.WriteLine("Active");

    String result = writer.ToString();
    Console::WriteLine("Buffered Report:\n{0}", result);

    return 0;
}
```
