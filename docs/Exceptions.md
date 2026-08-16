# Exceptions

**Namespace:** `DotNetDupe::System` & `DotNetDupe::System::IO`  
**Header:** `#include "System/Exception.h"`

Defines the base error class and derived runtime exceptions for all DotNetDupe library components. In compliance with Quality Gate Constraint #7, DotNetDupe strictly throws library exceptions derived from `Exception`.

---

## Complete Exception Hierarchy

```text
Exception (System/Exception.h)
 ├── SystemException (System/SystemException.h)
 │    ├── ArgumentException (System/ArgumentException.h)
 │    │    ├── ArgumentNullException (System/ArgumentNullException.h)
 │    │    └── ArgumentOutOfRangeException (System/ArgumentOutOfRangeException.h)
 │    ├── InvalidOperationException (System/InvalidOperationException.h)
 │    │    └── ObjectDisposedException (System/ObjectDisposedException.h)
 │    ├── NullReferenceException (System/NullReferenceException.h)
 │    ├── NotImplementedException (System/NotImplementedException.h)
 │    ├── NotSupportedException (System/NotSupportedException.h)
 │    ├── PlatformNotSupportedException (System/PlatformNotSupportedException.h)
 │    ├── FormatException (System/FormatException.h)
 │    ├── ArithmeticException (System/ArithmeticException.h)
 │    │    └── OverflowException (System/OverflowException.h)
 │    ├── TimeoutException (System/TimeoutException.h)
 │    ├── OperationCanceledException (System/OperationCanceledException.h)
 │    ├── UnauthorizedAccessException (System/UnauthorizedAccessException.h)
 │    ├── OutOfMemoryException (System/OutOfMemoryException.h)
 │    ├── AggregateException (System/AggregateException.h)
 │    └── IOException (System/IO/IOException.h)
 │         ├── FileNotFoundException (System/IO/FileNotFoundException.h)
 │         ├── DirectoryNotFoundException (System/IO/DirectoryNotFoundException.h)
 │         └── EndOfStreamException (System/IO/EndOfStreamException.h)
 ├── SocketException (System/Net/Sockets/SocketException.h)
 └── JsonException (System/Text/Json/JsonException.h)
```

---

## Base `Exception` Class

### Syntax
```cpp
class Exception : public std::runtime_error;
```

### Constructors & Assignment
- `Exception()`: Initializes a new instance of the `Exception` class with a default system message.
- `Exception(const String& sMessage)`: Initializes a new instance of the `Exception` class with a specified error message.
- `Exception(const String& sMessage, const Exception& innerException)`: Initializes a new instance of the `Exception` class with a specified error message and a reference to the inner exception that is the cause of this exception.
- `Exception(const Exception& other)`: Copy constructor.
- `Exception& operator=(const Exception& other)`: Copy assignment operator.
- `virtual ~Exception()`: Virtual destructor.

### Member Functions
- `const char* What() const`: Gets a message that describes the current exception.

---

## System & Collections Exceptions

| Exception Class | Header | Thrown When |
|---|---|---|
| `ArgumentException` | `#include "System/ArgumentException.h"` | An invalid argument is passed to a method or when a duplicate key is added to `Dictionary` / `SortedDictionary`. |
| `ArgumentNullException` | `#include "System/ArgumentNullException.h"` | A pointer or mandatory argument is `nullptr`. |
| `ArgumentOutOfRangeException` | `#include "System/ArgumentOutOfRangeException.h"` | An index or offset is negative or beyond the collection bounds in `List<T>`, `Array<T>`, etc. |
| `InvalidOperationException` | `#include "System/InvalidOperationException.h"` | Calling `Dequeue()` / `Pop()` / `Take()` on an empty collection, or mutating while in an invalid state. |
| `ObjectDisposedException` | `#include "System/ObjectDisposedException.h"` | Accessing an object after `Dispose()` has been called (e.g. on streams or readers). |
| `FormatException` | `#include "System/FormatException.h"` | String parsing / formatting fails (e.g. `DateTime::Parse`, `Convert::ToInt32`). |
| `OverflowException` | `#include "System/OverflowException.h"` | An arithmetic or conversion operation causes an overflow. |
| `NotImplementedException` | `#include "System/NotImplementedException.h"` | A requested interface method or functionality is not implemented. |
| `NotSupportedException` | `#include "System/NotSupportedException.h"` | An invoked method is not supported by the underlying implementation. |
| `PlatformNotSupportedException` | `#include "System/PlatformNotSupportedException.h"` | A feature is invoked on an unsupported operating system platform. |
| `TimeoutException` | `#include "System/TimeoutException.h"` | A blocking or synchronization operation exceeds its timeout interval. |
| `OperationCanceledException` | `#include "System/OperationCanceledException.h"` | An asynchronous or queued task is canceled. |
| `UnauthorizedAccessException` | `#include "System/UnauthorizedAccessException.h"` | The operating system denies access due to an I/O permission or security error. |

---

## I/O Namespace Exceptions (`DotNetDupe::System::IO`)

| Exception Class | Header | Thrown When |
|---|---|---|
| `IOException` | `#include "System/IO/IOException.h"` | An I/O error occurs while reading, writing, or manipulating a stream or file. |
| `FileNotFoundException` | `#include "System/IO/FileNotFoundException.h"` | An attempt to access a file that does not exist on disk fails. |
| `DirectoryNotFoundException` | `#include "System/IO/DirectoryNotFoundException.h"` | Part of a file or directory path cannot be found. |
| `EndOfStreamException` | `#include "System/IO/EndOfStreamException.h"` | Reading is attempted past the end of a stream. |

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/List.h"
#include "System/IO/File.h"
#include "System/IO/FileNotFoundException.h"
#include "System/ArgumentOutOfRangeException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::IO;

int main() {
    // Collection boundary checking
    try {
        List<int> listNumbers;
        listNumbers.Add(10);
        int iInvalid = listNumbers[5]; // Index 5 out of range
    } catch (const ArgumentOutOfRangeException& ex) {
        Console::WriteLine("Caught Collection Exception: {0}", ex.What());
    }

    // File I/O checking
    try {
        String sData = File::ReadAllText("non_existent_file.txt");
    } catch (const FileNotFoundException& ex) {
        Console::WriteLine("Caught File Not Found: {0}", ex.What());
    } catch (const IOException& ex) {
        Console::WriteLine("Caught Generic I/O Exception: {0}", ex.What());
    }

    return 0;
}
```
