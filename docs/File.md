### class `File`

Provides static methods for the creation, copying, deletion, moving, and opening of a single file, and aids in the creation of `FileStream` objects.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/IO/File.h"
#include "../DotNetDupe/String.h"

int main() {
    const DotNetDupe::System::String testFile(_T("test.txt"));
    const DotNetDupe::System::String content(_T("Hello, World!"));
    DotNetDupe::System::IO::File::WriteAllText(testFile, content);
    DotNetDupe::System::String readContent = DotNetDupe::System::IO::File::ReadAllText(testFile);
    std::wcout << L"Read from file: " << readContent.GetRawString() << std::endl;
    DotNetDupe::System::IO::File::Delete(testFile);

    return 0;
}
```

#### Methods

##### `static bool Exists(const String& path)`

Determines whether the specified file exists.

##### `static String ReadAllText(const String& path)`

Opens a text file, reads all the text in the file into a string, and then closes the file.

##### `static void WriteAllText(const String& path, const String& contents)`

Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten.

##### `static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite)`

Copies an existing file to a new file. Overwriting a file of the same name is allowed.

##### `static void Move(const String& sourceFileName, const String& destFileName)`

Moves a specified file to a new location, providing the option to specify a new file name.

##### `static void Delete(const String& path)`

Deletes the specified file.

##### `static void AppendAllText(const String& path, const String& contents)`

Appends the specified string to the file, creating the file if it does not already exist.

##### `static void AppendAllLines(const String& path, const std::vector<String>& contents)`

Appends lines to a file, and then closes the file.

##### `static std::vector<String> ReadAllLines(const String& path)`

Opens a text file, reads all lines of the file, and then closes the file.

##### `static void WriteAllLines(const String& path, const std::vector<String>& contents)`

Creates a new file, writes a collection of strings to the file, and then closes the file.

##### `static void Create(const String& path)`

Creates or overwrites a file in the specified path.

##### `static int GetAttributes(const String& path)`

Gets the `FileAttributes` of the file on the path.

##### `static void SetAttributes(const String& path, int fileAttributes)`

Sets the specified `FileAttributes` of the file on the specified path.
