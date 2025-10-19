### class `Path`

A class for path-related operations.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Path.h"
#include "../DotNetDupe/String.h"

int main() {
    DotNetDupe::System::String path1 = _T("C:\\Users\\Test");
    DotNetDupe::System::String path2 = _T("file.txt");
    DotNetDupe::System::String combinedPath = DotNetDupe::System::IO::Path::Combine({path1, path2});
    std::wcout << L"Combined Path: " << combinedPath.GetRawString() << std::endl;
    std::wcout << L"File Extension: " << DotNetDupe::System::IO::Path::GetExtension(combinedPath).GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `static String ChangeExtension(const String &filePath, const String &extension)`

Changes the extension of a path string.

##### `static String Combine(const std::initializer_list<String> paths)`

Combines multiple strings into a path.

##### `static bool EndsInDirectorySeparator(const String &filePath)`

Determines whether the path ends in a directory separator.

##### `static bool Exists(const String &filePath)`

Determines whether the given path refers to an existing file or directory.

##### `static String GetDirectoryName(const String &filePath)`

Returns the directory information for the specified path.

##### `static String GetFileName(const String &filePath)`

Returns the file name and extension of the specified path string.

##### `static String GetExtension(const String &filePath)`

Returns the extension of the specified path string.

##### `static String GetFileNameWithoutExtension(const String &filePath)`

Returns the file name of the specified path string without the extension.

##### `static String GetFullPath(const String &path)`

Returns the absolute path for the specified path string.

##### `static std::vector<TCHAR> GetInvalidFileNameChars()`

Gets an array of characters that are not allowed in file names.

##### `static std::vector<TCHAR> GetInvalidPathChars()`

Gets an array of characters that are not allowed in path names.
