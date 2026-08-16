# File

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/File.h"`

Provides static methods for creating, copying, deleting, moving, reading, and writing files.

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
  - `sPath` (`const String&`): The file path to check.
- **Returns:**
  - `bool`: `true` if `sPath` contains the name of an existing file; otherwise, `false`.

```cpp
bool bExists = File::Exists("config.json");
```

### `static String ReadAllText(const String& sPath)`
Opens a text file, reads all the text in the file, and then closes the file.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid or empty.
  - `FileNotFoundException`: If the file cannot be found.
  - `IOException`: If an I/O error occurs while reading the file.

```cpp
String sContent = File::ReadAllText("config.json");
```

### `static void WriteAllText(const String& sPath, const String& sContents)`
Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid or empty.
  - `IOException`: If an I/O error occurs while writing the file.

```cpp
File::WriteAllText("output.log", "Operation completed successfully.\n");
```

### `static Array<String> ReadAllLines(const String& sPath)`
Opens a text file, reads all lines of the file, and then closes the file.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `FileNotFoundException`: If the file does not exist.
  - `IOException`: If an I/O error occurs.

### `static void WriteAllLines(const String& sPath, const Array<String>& sContents)`
Creates a new file, writes a collection of strings to the file, and then closes the file.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `IOException`: If an I/O error occurs.

### `static void AppendAllText(const String& sPath, const String& sContents)`
Appends the specified string to the file, creating the file if it does not already exist.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `IOException`: If an I/O error occurs.

### `static void AppendAllLines(const String& sPath, const Array<String>& sContents)`
Appends lines to a file, and then closes the file. If the specified file does not exist, this method creates a file.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `IOException`: If an I/O error occurs.

### `static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite)`
Copies an existing file to a new file. Overwriting a file of the same name is allowed if `bOverwrite` is `true`.
- **Throws:**
  - `ArgumentException`: If source or destination path is invalid.
  - `FileNotFoundException`: If source file does not exist.
  - `IOException`: If destination file exists and `bOverwrite` is `false`, or an I/O error occurs.

### `static void Move(const String& sSourceFileName, const String& sDestFileName)`
Moves a specified file to a new location.
- **Throws:**
  - `ArgumentException`: If source or destination path is invalid.
  - `FileNotFoundException`: If source file does not exist.
  - `IOException`: If an I/O error occurs.

### `static void Delete(const String& sPath)`
Deletes the specified file.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `IOException`: If an I/O error occurs.

### `static void Create(const String& sPath)`
Creates or overwrites a file in the specified path.
- **Throws:**
  - `ArgumentException`: If `sPath` is invalid.
  - `IOException`: If an I/O error occurs.

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
    String sPath = "sample.txt";

    File::WriteAllText(sPath, "Line 1: Alpha\nLine 2: Beta\nLine 3: Gamma");
    Console::WriteLine("File created: {0}", File::Exists(sPath));

    Array<String> arrLines = File::ReadAllLines(sPath);
    Console::WriteLine("Read {0} lines from file:", arrLines.GetLength());
    for (int iIdx = 0; iIdx < arrLines.GetLength(); ++iIdx) {
        Console::WriteLine(" [{0}] {1}", iIdx, arrLines[iIdx]);
    }

    File::Delete(sPath);
    Console::WriteLine("File deleted. Exists: {0}", File::Exists(sPath));

    return 0;
}
```
