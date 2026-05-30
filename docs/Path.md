### class `Path`

A class for path-related operations.

#### Methods

##### `static String ChangeExtension(const String &sFilePath, const String &sExtension)`

Changes the extension of a path string.

**Parameters:**
- `sFilePath`: The path information to modify.
- `sExtension`: The new extension (with or without a leading period). Specify `null` to remove an existing extension from `sFilePath`.

**Returns:**
- The modified path information.

**Usage:**
```cpp
String sResult = Path::ChangeExtension("file.txt", ".bak"); // "file.bak"
```

##### `static String Combine(const std::initializer_list<String> arrPaths)`

Combines multiple strings into a path.

**Parameters:**
- `arrPaths`: A collection of parts of the path.

**Returns:**
- The combined paths.

**Usage:**
```cpp
String sCombined = Path::Combine({ "C:", "Users", "test.txt" });
```

##### `static bool EndsInDirectorySeparator(const String &sFilePath)`

Determines whether the path ends in a directory separator.

**Parameters:**
- `sFilePath`: The path to analyze.

**Returns:**
- `true` if the path ends in a directory separator; otherwise, `false`.

**Usage:**
```cpp
bool bResult = Path::EndsInDirectorySeparator("C:\\Temp\\"); // true
```

##### `static bool Exists(const String &sFilePath)`

Determines whether the given path refers to an existing file or directory.

**Parameters:**
- `sFilePath`: The path to test.

**Returns:**
- `true` if the path refers to an existing file or directory; otherwise, `false`.

**Usage:**
```cpp
if (Path::Exists("test.txt")) { /* ... */ }
```

##### `static String GetDirectoryName(const String &sFilePath)`

Returns the directory information for the specified path.

**Parameters:**
- `sFilePath`: The path of a file or directory.

**Returns:**
- Directory information for `sFilePath`, or an empty string if `sFilePath` denotes a root directory or is null.

**Usage:**
```cpp
String sDir = Path::GetDirectoryName("C:\\Temp\\test.txt"); // "C:\\Temp"
```

##### `static String GetFileName(const String &sFilePath)`

Returns the file name and extension of the specified path string.

**Parameters:**
- `sFilePath`: The path string from which to obtain the file name and extension.

**Returns:**
- The characters after the last directory separator in `sFilePath`.

**Usage:**
```cpp
String sFileName = Path::GetFileName("C:\\Temp\\test.txt"); // "test.txt"
```

##### `static String GetExtension(const String &sFilePath)`

Returns the extension of the specified path string.

**Parameters:**
- `sFilePath`: The path string from which to get the extension.

**Returns:**
- The extension of the specified path (including the period "."), or an empty string.

**Usage:**
```cpp
String sExt = Path::GetExtension("test.txt"); // ".txt"
```

##### `static String GetFileNameWithoutExtension(const String &sFilePath)`

Returns the file name of the specified path string without the extension.

**Parameters:**
- `sFilePath`: The path of the file.

**Returns:**
- The string returned by `GetFileName`, minus the last period and all characters following it.

**Usage:**
```cpp
String sName = Path::GetFileNameWithoutExtension("test.txt"); // "test"
```

##### `static String GetFullPath(const String &sPath)`

Returns the absolute path for the specified path string.

**Parameters:**
- `sPath`: The file or directory for which to obtain absolute path information.

**Returns:**
- The fully qualified location of `sPath`.

**Usage:**
```cpp
String sFullPath = Path::GetFullPath("test.txt");
```

##### `static Array<char> GetInvalidFileNameChars()`

Gets an array of characters that are not allowed in file names.

**Returns:**
- An `Array<char>` containing the characters that are not allowed in file names.

**Usage:**
```cpp
auto arrInvalid = Path::GetInvalidFileNameChars();
```

##### `static Array<char> GetInvalidPathChars()`

Gets an array of characters that are not allowed in path names.

**Returns:**
- An `Array<char>` containing the characters that are not allowed in path names.

**Usage:**
```cpp
auto arrInvalid = Path::GetInvalidPathChars();
```

