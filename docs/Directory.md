# Directory

The `Directory` class in `DotNetDupe::System::IO` exposes static methods for creating, moving, deleting, enumerating, and querying directories and files.

## Namespace
```cpp
namespace DotNetDupe::System::IO
```

## Syntax & Class Definition
```cpp
class Directory : public Object {
public:
    static bool Exists(const String& sPath);
    static void CreateDirectory(const String& sPath);
    static void CreateDirectory(const String& sPath, bool bRecursive);
    static void Delete(const String& sPath);
    static void Delete(const String& sPath, bool bRecursive);
    static void Move(const String& sSourceDirName, const String& sDestDirName);
    
    static Array<String> GetFiles(const String& sPath);
    static Array<String> GetFiles(const String& sPath, const String& sSearchPattern);
    
    static Array<String> GetDirectories(const String& sPath);
    static Array<String> GetDirectories(const String& sPath, const String& sSearchPattern);
    
    static Array<String> GetFileSystemEntries(const String& sPath);
    static Array<String> GetFileSystemEntries(const String& sPath, const String& sSearchPattern);
    
    static String GetCurrentDirectory();
    static void SetCurrentDirectory(const String& sPath);
    
    static String GetDirectoryRoot(const String& sPath);
    static DateTimeOffset GetCreationTime(const String& sPath);
    static DateTimeOffset GetLastWriteTime(const String& sPath);
    static DateTimeOffset GetLastAccessTime(const String& sPath);
};
```

## Methods

| Method | Description |
|---|---|
| `Exists(path)` | Checks if the directory exists. |
| `CreateDirectory(path)` | Creates all directories and subdirectories in the specified path. |
| `Delete(path, [recursive])` | Deletes a directory and optionally its contents. |
| `Move(source, dest)` | Moves a directory to a new location. |
| `GetFiles(path, [pattern])` | Returns array of file names matching pattern. |
| `GetDirectories(path, [pattern])` | Returns array of subdirectories matching pattern. |
| `GetFileSystemEntries(path, [pattern])` | Returns array of all file and directory entries matching pattern. |
| `GetCurrentDirectory()` | Gets current working directory. |
| `SetCurrentDirectory(path)` | Sets current working directory. |
| `GetDirectoryRoot(path)` | Gets root volume string for given path. |
| `GetLastWriteTime(path)` | Returns last modification timestamp. |

## Complete Usage Example

```cpp
#include "System/Console.h"
#include "System/Convert.h"
#include "System/IO/Directory.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String currentDir = Directory::GetCurrentDirectory();
    Console::WriteLine(String("Working Dir: ") + currentDir);

    String targetFolder = Path::Combine({ currentDir, "SampleFolder" });
    if (!Directory::Exists(targetFolder)) {
        Directory::CreateDirectory(targetFolder);
    }

    String filePath = Path::Combine({ targetFolder, "data.txt" });
    File::WriteAllText(filePath, "Hello World");

    Array<String> files = Directory::GetFiles(targetFolder);
    Console::WriteLine(String("Files count: ") + Convert::ToString(files.GetLength()));

    Directory::Delete(targetFolder, true);
    return 0;
}
```
