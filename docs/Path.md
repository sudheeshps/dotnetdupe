# Path

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/Path.h"`

Performs operations on `String` instances that contain file or directory path information. These operations are performed in a cross-platform manner.

---

## Syntax

```cpp
class Path;
```

---

## Static Methods & Path Operators

### `static String Combine(const std::initializer_list<String> sPaths)`
Combines multiple path strings into a single path using the appropriate directory separator char.

```cpp
String fullPath = Path::Combine({ "C:\\App", "Data", "config.json" });
```

### `static String Join(const std::initializer_list<String> sPaths)`
Concatenates an array of paths into a single path.

### `static bool TryJoin(const std::initializer_list<String> sPaths, String& sResult)`
Attempts to concatenate paths into `sResult`.

### `static String ChangeExtension(const String& sFilePath, const String& sExtension)`
Changes the extension of a path string.

```cpp
String imgPath = Path::ChangeExtension("report.doc", ".pdf"); // "report.pdf"
```

### `static String GetDirectoryName(const String& sFilePath)`
Returns the directory information for the specified path string.

### `static String GetFileName(const String& sFilePath)`
Returns the file name and extension of the specified path string.

### `static String GetFileNameWithoutExtension(const String& sFilePath)`
Returns the file name of the specified path string without the extension.

### `static String GetExtension(const String& sFilePath)`
Returns the extension (including the period `"."`) of the specified path string.

### `static bool HasExtension(const String& sPath)`
Determines whether a path includes a file name extension.

### `static String GetFullPath(const String& sPath)`
Returns the absolute path for the specified path string.

### `static String GetRelativePath(const String& sRelativeTo, const String& sPath)`
Returns a relative path from one path to another.

### `static String GetPathRoot(const String& sPath)`
Gets the root directory information from the path.

### `static bool IsPathRooted(const String& sPath)`
Gets a value indicating whether the specified path string contains a root.

### `static bool IsPathFullyQualified(const String& sPath)`
Returns a value that indicates whether the path is fully qualified.

### `static String GetTempPath()`
Returns the path of the current user's temporary folder.

### `static String GetTempFileName()`
Creates a uniquely named, zero-byte temporary file on disk and returns the full path of that file.

### `static String GetRandomFileName()`
Returns a random folder name or file name.

### `static bool EndsInDirectorySeparator(const String& sFilePath)`
Returns a value that indicates whether the specified file path ends in a directory separator.

### `static String TrimEndingDirectorySeparator(const String& sPath)`
Trims one trailing directory separator beyond the root of the specified path.

### `static bool Exists(const String& sFilePath)`
Determines whether the given path refers to an existing file or directory.

---

## Static Separator Constants

- `static char GetDirectorySeparatorChar()`: Primary separator character (`\` on Windows, `/` on Linux).
- `static char GetAltDirectorySeparatorChar()`: Alternate separator character (`/` on Windows).
- `static char GetVolumeSeparatorChar()`: Volume separator (`:` on Windows).
- `static char GetPathSeparator()`: Path separator in environment variables (`;` on Windows, `:` on POSIX).
- `static Array<char> GetInvalidFileNameChars()`: Returns an array containing characters not allowed in file names.
- `static Array<char> GetInvalidPathChars()`: Returns an array containing characters not allowed in path names.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IO/Path.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    String filePath = "C:\\Projects\\DotNetDupe\\Include\\System\\String.h";

    Console::WriteLine("Directory: {0}", Path::GetDirectoryName(filePath));
    Console::WriteLine("File Name: {0}", Path::GetFileName(filePath));
    Console::WriteLine("No Ext:    {0}", Path::GetFileNameWithoutExtension(filePath));
    Console::WriteLine("Extension: {0}", Path::GetExtension(filePath));

    String tempLog = Path::Combine({ Path::GetTempPath(), "test.log" });
    Console::WriteLine("Combined Temp: {0}", tempLog);

    return 0;
}
```
