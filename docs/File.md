### class `File`

Provides static methods for the creation, copying, deletion, moving, and opening of a single file.

#### Methods

##### `static bool Exists(const String& sPath)`

Determines whether the specified file exists.

**Parameters:**
- `sPath`: The file to check.

**Returns:**
- `true` if the caller has the required permissions and `sPath` contains the name of an existing file; otherwise, `false`.

**Usage:**
```cpp
if (File::Exists("test.txt")) { /* ... */ }
```

##### `static String ReadAllText(const String& sPath)`

Opens a text file, reads all the text in the file into a string, and then closes the file.

**Parameters:**
- `sPath`: The file to open for reading.

**Returns:**
- A string containing all the text in the file.

**Usage:**
```cpp
String sContent = File::ReadAllText("test.txt");
```

##### `static void WriteAllText(const String& sPath, const String& sContents)`

Creates a new file, writes the specified string to the file, and then closes the file. If the target file already exists, it is overwritten.

**Parameters:**
- `sPath`: The file to write to.
- `sContents`: The string to write to the file.

**Usage:**
```cpp
File::WriteAllText("test.txt", "Hello World");
```

##### `static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite)`

Copies an existing file to a new file. Overwriting a file of the same name is allowed.

**Parameters:**
- `sSourceFileName`: The file to copy.
- `sDestFileName`: The name of the destination file. This cannot be a directory.
- `bOverwrite`: `true` if the destination file can be overwritten; otherwise, `false`.

**Usage:**
```cpp
File::Copy("old.txt", "new.txt", true);
```

##### `static void Move(const String& sSourceFileName, const String& sDestFileName)`

Moves a specified file to a new location, providing the option to specify a new file name.

**Parameters:**
- `sSourceFileName`: The name of the file to move.
- `sDestFileName`: The new path for the file.

**Usage:**
```cpp
File::Move("file.txt", "C:\\Backup\\file.txt");
```

##### `static void Delete(const String& sPath)`

Deletes the specified file.

**Parameters:**
- `sPath`: The name of the file to be deleted. Wildcard characters are not supported.

**Usage:**
```cpp
File::Delete("temp.txt");
```

##### `static void AppendAllText(const String& sPath, const String& sContents)`

Appends the specified string to the file, creating the file if it does not already exist.

**Parameters:**
- `sPath`: The file to append the specified string to.
- `sContents`: The string to append to the file.

**Usage:**
```cpp
File::AppendAllText("log.txt", "New log entry\n");
```

##### `static Array<String> ReadAllLines(const String& sPath)`

Opens a text file, reads all lines of the file into a string array, and then closes the file.

**Parameters:**
- `sPath`: The file to open for reading.

**Returns:**
- An `Array<String>` containing all lines of the file.

**Usage:**
```cpp
auto arrLines = File::ReadAllLines("test.txt");
```

##### `static void WriteAllLines(const String& sPath, const Array<String>& arrContents)`

Creates a new file, writes a collection of strings to the file, and then closes the file.

**Parameters:**
- `sPath`: The file to write to.
- `arrContents`: The collection of strings to write to the file.

**Usage:**
```cpp
Array<String> arrLines = { "Line 1", "Line 2" };
File::WriteAllLines("test.txt", arrLines);
```

##### `static void Create(const String& sPath)`

Creates or overwrites a file in the specified path.

**Parameters:**
- `sPath`: The path and name of the file to create.

**Usage:**
```cpp
File::Create("newfile.txt");
```

##### `static bool GetAttributes(const String& sPath, FileAttributes& attrAttributes)`

Gets the attributes of the file on the path.

**Parameters:**
- `sPath`: The path to the file.
- `attrAttributes`: An output parameter that will hold the attributes of the file.

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
FileAttributes attrAttributes;
if (File::GetAttributes("test.txt", attrAttributes)) {
    if ((static_cast<int>(attrAttributes) & static_cast<int>(FileAttributes::ReadOnly)) != 0) {
        Console::WriteLine("File is read-only");
    }
}
```

##### `static bool SetAttributes(const String& sPath, FileAttributes attrAttributes)`

Sets the specified attributes of the file on the specified path.

**Parameters:**
- `sPath`: The path to the file.
- `attrAttributes`: A bitwise combination of the `FileAttributes` enumeration values.

**Returns:**
- `true` if the attributes were successfully set; otherwise, `false`.

**Platform-Specific Behavior:**
- **Windows**: Maps directly to Win32 `SetFileAttributesW`.
- **Linux/POSIX**: Uses `std::filesystem::permissions`.
    - `ReadOnly`: If set, removes all write permissions (`owner_write`, `group_write`, `others_write`). If NOT set, adds `owner_write`.
    - Other flags like `Hidden` or `Directory` cannot be set via this method on Linux as they are intrinsic to the name or file type.

**Usage:**
```cpp
File::SetAttributes("test.txt", FileAttributes::ReadOnly);
```

