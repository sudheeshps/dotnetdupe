# File

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/File.h"`

Provides static methods for creating, copying, deleting, moving, reading, and opening files, and aids in the creation of `FileStream` objects.

---

## Syntax

```cpp
enum class FileAttributes {
    Normal = 0, ReadOnly = 1, Hidden = 2, System = 4, Directory = 16,
    Archive = 32, Device = 64, Temporary = 128, SparseFile = 256,
    ReparsePoint = 512, Compressed = 1024, Offline = 2048,
    NotContentIndexed = 4096, Encrypted = 8192, IntegrityStream = 16384,
    NoScrubData = 32768
};

class File : public Object;
```

---

## Static Methods

### `static bool Exists(const String& sPath)`
Determines whether the specified file exists.
- **Parameters:**
  - `sPath` (`const String&`): The file to check.
- **Returns:**
  - `bool`: `true` if the caller has the required permissions and `sPath` contains the name of an existing file; otherwise, `false`.

```cpp
bool bExists = File::Exists("config.json");
```

### `static String ReadAllText(const String& sPath)`
Opens a text file, reads all the text in the file, and then closes the file.
- **Throws:**
  - `IOException`: If the file cannot be opened or read.

```cpp
String content = File::ReadAllText("config.json");
```

### `static void WriteAllText(const String& sPath, const String& sContents)`
Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten.

```cpp
File::WriteAllText("output.log", "Operation completed successfully.\n");
```

### `static Array<String> ReadAllLines(const String& sPath)`
Opens a text file, reads all lines of the file, and then closes the file.
- **Returns:**
  - `Array<String>`: A string array containing all lines of the file.

### `static void WriteAllLines(const String& sPath, const Array<String>& sContents)`
Creates a new file, writes a collection of strings to the file, and then closes the file.

### `static void AppendAllText(const String& sPath, const String& sContents)`
Appends the specified string to the file, creating the file if it does not already exist.

### `static void AppendAllLines(const String& sPath, const Array<String>& sContents)`
Appends lines to a file, and then closes the file. If the specified file does not exist, this method creates a file.

### `static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite = false)`
Copies an existing file to a new file. Overwriting a file of the same name is allowed if `bOverwrite` is `true`.

### `static void Move(const String& sSourceFileName, const String& sDestFileName)`
Moves a specified file to a new location, providing the option to specify a new file name.

### `static void Delete(const String& sPath)`
Deletes the specified file.

### `static void Create(const String& sPath)`
Creates or overwrites a file in the specified path.

### `static bool GetAttributes(const String& sPath, FileAttributes& attrAttributes)`
Gets the `FileAttributes` of the file on the path.

### `static bool SetAttributes(const String& sPath, FileAttributes attrAttributes)`
Sets the specified `FileAttributes` of the file on the specified path.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/File.h"
#include "System/Array.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String path = "sample.txt";

    File::WriteAllText(path, "Line 1: Alpha\nLine 2: Beta\nLine 3: Gamma");
    Console::WriteLine("File created: {0}", File::Exists(path));

    Array<String> lines = File::ReadAllLines(path);
    Console::WriteLine("Read {0} lines from file:", lines.GetLength());
    for (int i = 0; i < lines.GetLength(); ++i) {
        Console::WriteLine(" [{0}] {1}", i, lines[i]);
    }

    File::Delete(path);
    Console::WriteLine("File deleted. Exists: {0}", File::Exists(path));

    return 0;
}
```
