### class `File`

Provides static methods for the creation, copying, deletion, moving, and opening of a single file.

#### Methods

##### `static bool Exists(const String& path)`

Determines whether the specified file exists.

**Parameters:**
- `path`: The file to check.

**Returns:**
- `true` if the caller has the required permissions and `path` contains the name of an existing file; otherwise, `false`.

**Usage:**
```cpp
if (File::Exists(_T("test.txt"))) { /* ... */ }
```

##### `static String ReadAllText(const String& path)`

Opens a text file, reads all the text in the file into a string, and then closes the file.

**Parameters:**
- `path`: The file to open for reading.

**Returns:**
- A string containing all the text in the file.

**Usage:**
```cpp
String content = File::ReadAllText(_T("test.txt"));
```

##### `static void WriteAllText(const String& path, const String& contents)`

Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten.

**Parameters:**
- `path`: The file to write to.
- `contents`: The string to write to the file.

**Usage:**
```cpp
File::WriteAllText(_T("test.txt"), _T("Hello World"));
```

##### `static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite)`

Copies an existing file to a new file. Overwriting a file of the same name is allowed.

**Parameters:**
- `sourceFileName`: The file to copy.
- `destFileName`: The name of the destination file. This cannot be a directory.
- `overwrite`: `true` if the destination file can be overwritten; otherwise, `false`.

**Usage:**
```cpp
File::Copy(_T("old.txt"), _T("new.txt"), true);
```

##### `static void Move(const String& sourceFileName, const String& destFileName)`

Moves a specified file to a new location, providing the option to specify a new file name.

**Parameters:**
- `sourceFileName`: The name of the file to move.
- `destFileName`: The new path for the file.

**Usage:**
```cpp
File::Move(_T("file.txt"), _T("C:\\Backup\\file.txt"));
```

##### `static void Delete(const String& path)`

Deletes the specified file.

**Parameters:**
- `path`: The name of the file to be deleted. Wildcard characters are not supported.

**Usage:**
```cpp
File::Delete(_T("temp.txt"));
```

##### `static void AppendAllText(const String& path, const String& contents)`

Appends the specified string to the file, creating the file if it does not already exist.

**Parameters:**
- `path`: The file to append the specified string to.
- `contents`: The string to append to the file.

**Usage:**
```cpp
File::AppendAllText(_T("log.txt"), _T("New log entry\n"));
```

##### `static Array<String> ReadAllLines(const String& path)`

Opens a text file, reads all lines of the file into a string array, and then closes the file.

**Parameters:**
- `path`: The file to open for reading.

**Returns:**
- An `Array<String>` containing all lines of the file.

**Usage:**
```cpp
auto lines = File::ReadAllLines(_T("test.txt"));
```

##### `static void WriteAllLines(const String& path, const Array<String>& contents)`

Creates a new file, writes a collection of strings to the file, and then closes the file.

**Parameters:**
- `path`: The file to write to.
- `contents`: The collection of strings to write to the file.

**Usage:**
```cpp
Array<String> lines = { _T("Line 1"), _T("Line 2") };
File::WriteAllLines(_T("test.txt"), lines);
```

##### `static void Create(const String& path)`

Creates or overwrites a file in the specified path.

**Parameters:**
- `path`: The path and name of the file to create.

**Usage:**
```cpp
File::Create(_T("newfile.txt"));
```

##### `static bool GetAttributes(const String& path, FileAttributes& fileAttributes)`

Gets the attributes of the file on the path.

**Parameters:**
- `path`: The path to the file.
- `fileAttributes`: An output parameter that will hold the attributes of the file.

**Returns:**
- `true` if the attributes were successfully retrieved; otherwise, `false`.

**Platform-Specific Behavior:**
- **Windows**: Maps directly to Win32 `GetFileAttributesW`. All `FileAttributes` values are supported.
- **Linux/POSIX**: Uses `std::filesystem::status`. 
    - `ReadOnly`: Set if the file does not have `owner_write` permission.
    - `Directory`: Set if the path refers to a directory.
    - `Hidden`: Set if the filename starts with a period (`.`).
    - `Normal`: Set if no other mapped attributes are present.

**Usage:**
```cpp
FileAttributes attrs;
if (File::GetAttributes(_T("test.txt"), attrs)) {
    if ((static_cast<int>(attrs) & static_cast<int>(FileAttributes::ReadOnly)) != 0) {
        Console::WriteLine("File is read-only");
    }
}
```

##### `static bool SetAttributes(const String& path, FileAttributes fileAttributes)`

Sets the specified attributes of the file on the specified path.

**Parameters:**
- `path`: The path to the file.
- `fileAttributes`: A bitwise combination of the `FileAttributes` enumeration values.

**Returns:**
- `true` if the attributes were successfully set; otherwise, `false`.

**Platform-Specific Behavior:**
- **Windows**: Maps directly to Win32 `SetFileAttributesW`.
- **Linux/POSIX**: Uses `std::filesystem::permissions`.
    - `ReadOnly`: If set, removes all write permissions (`owner_write`, `group_write`, `others_write`). If NOT set, adds `owner_write`.
    - Other flags like `Hidden` or `Directory` cannot be set via this method on Linux as they are intrinsic to the name or file type.

**Usage:**
```cpp
File::SetAttributes(_T("test.txt"), FileAttributes::ReadOnly);
```
