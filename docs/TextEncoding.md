# TextEncoding &amp; Encoding

**Namespace:** `DotNetDupe::System::Text`  
**Header:** `#include "System/Text/TextEncoding.h"`

Represents character encodings (UTF-8, ASCII, Unicode), providing bidirectional transcoding between `String` objects and byte arrays.

---

## `Encoding` (Abstract Base Class)

### Syntax
```cpp
class Encoding : public Object;
using EncodingPtr = SmartPointer<Encoding>;
```

### Pure Virtual Members
- `virtual Array<char> GetBytes(const String& s) = 0`: Encodes a set of characters from the specified `String` into an array of bytes.
- `virtual String GetString(const Array<char>& bytes) = 0`: Decodes an array of bytes into a `String`.
- `virtual String GetString(const char* bytes, int byteCount) = 0`: Decodes a sequence of bytes from the specified character pointer into a `String`.

---

## `UTF8Encoding` & `TextEncoding`

### Static Factory
- `static EncodingPtr TextEncoding::UTF8()`: Returns an encoding for the UTF-8 format.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Text/TextEncoding.h"
#include "System/Array.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

int main() {
    auto utf8 = TextEncoding::UTF8();

    String original = "DotNetDupe UTF-8 Encoding 🚀";
    Array<char> bytes = utf8->GetBytes(original);
    Console::WriteLine("Encoded to {0} bytes.", bytes.GetLength());

    String decoded = utf8->GetString(bytes);
    Console::WriteLine("Decoded String: {0}", decoded);

    return 0;
}
```
