# Directory

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/Directory.h"`

Exposes static methods for creating (including recursive creation), moving, enumerating, and deleting directories and subdirectories.

---

## Syntax

```cpp
class Directory : public Object;
```

---

## Static Methods

### `static bool Exists(const String& sPath)`
Determines whether the given path refers to an existing directory on disk.

```cpp
bool bExists = Directory::Exists("C:\\Logs");
```

### `static void CreateDirectory(const String& sPath)` / `static void CreateDirectory(const String& sPath, bool bRecursive)`
Creates all directories and subdirectories in the specified path unless they already exist.
- **Parameters:**
  - `sPath` (`const String&`): The directory to create.
  - `bRecursive` (`bool`): If `true`, creates all intermediate parent directories along the path.

```cpp
Directory::CreateDirectory("var/log/myapp/archived", true);
```

### `static void Delete(const String& sPath)` / `static void Delete(const String& sPath, bool bRecursive)`
Deletes the specified directory and, if indicated, any subdirectories and files in the directory.
- **Throws:**
  - `IOException`: If the directory is not empty and `bRecursive` is `false`.

### `static void Move(const String& sSourceDirName, const String& sDestDirName)`
Moves a file or a directory and its contents to a new location.

### `static Array<String> GetFiles(const String& sPath)` / `static Array<String> GetFiles(const String& sPath, const String& sSearchPattern)`
Returns the names of files (including their paths) that match the specified search pattern in the specified directory.

```cpp
Array<String> arrTxtFiles = Directory::GetFiles("C:\\Logs", "*.txt");
```

### `static Array<String> GetDirectories(const String& sPath)` / `static Array<String> GetDirectories(const String& sPath, const String& sSearchPattern)`
Returns the names of subdirectories (including their paths) in the specified directory.

### `static Array<String> GetFileSystemEntries(const String& sPath)` / `static Array<String> GetFileSystemEntries(const String& sPath, const String& sSearchPattern)`
Returns the names of all files and subdirectories in the specified directory.

### `static String GetCurrentDirectory()`
Gets the current working directory of the application.

### `static void SetCurrentDirectory(const String& sPath)`
Sets the application's current working directory to the specified directory.

### `static String GetDirectoryRoot(const String& sPath)`
Returns the volume information, root information, or both for the specified path (e.g. `C:\` or `/`).

### `static DateTimeOffset GetCreationTime(const String& sPath)`
Gets the creation date and time of a directory.

### `static DateTimeOffset GetLastWriteTime(const String& sPath)`
Gets the date and time when the specified directory was last written to.

### `static DateTimeOffset GetLastAccessTime(const String& sPath)`
Gets the date and time when the specified directory was last accessed.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/Directory.h"
#include "System/Array.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String sWorkingDir = Directory::GetCurrentDirectory();
    Console::WriteLine("Current Working Directory: {0}", sWorkingDir);

    String sNewFolder = "TempLogs/2026/08";
    Directory::CreateDirectory(sNewFolder, true);
    Console::WriteLine("Created folder: {0} (Exists: {1})", sNewFolder, Directory::Exists(sNewFolder));

    Array<String> arrFiles = Directory::GetFiles(sWorkingDir);
    Console::WriteLine("Total files found: {0}", arrFiles.GetLength());

    Directory::Delete("TempLogs", true);
    Console::WriteLine("Cleaned up TempLogs.");

    return 0;
}
```
