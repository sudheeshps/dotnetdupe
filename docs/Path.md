### class `Path`

A class for path-related operations.

#### Methods

##### `static String ChangeExtension(const String &filePath, const String &extension)`

Changes the extension of a path string.

**Parameters:**
- `filePath`: The path information to modify.
- `extension`: The new extension (with or without a leading period). Specify `null` to remove an existing extension from `filePath`.

**Returns:**
- The modified path information.

**Usage:**
```cpp
String result = Path::ChangeExtension(_T("file.txt"), _T(".bak")); // "file.bak"
```

##### `static String Combine(const std::initializer_list<String> paths)`

Combines multiple strings into a path.

**Parameters:**
- `paths`: A collection of parts of the path.

**Returns:**
- The combined paths.

**Usage:**
```cpp
String combined = Path::Combine({ _T("C:"), _T("Users"), _T("test.txt") });
```

##### `static bool EndsInDirectorySeparator(const String &filePath)`

Determines whether the path ends in a directory separator.

**Parameters:**
- `filePath`: The path to analyze.

**Returns:**
- `true` if the path ends in a directory separator; otherwise, `false`.

**Usage:**
```cpp
bool result = Path::EndsInDirectorySeparator(_T("C:\\Temp\\")); // true
```

##### `static bool Exists(const String &filePath)`

Determines whether the given path refers to an existing file or directory.

**Parameters:**
- `filePath`: The path to test.

**Returns:**
- `true` if the path refers to an existing file or directory; otherwise, `false`.

**Usage:**
```cpp
if (Path::Exists(_T("test.txt"))) { /* ... */ }
```

##### `static String GetDirectoryName(const String &filePath)`

Returns the directory information for the specified path.

**Parameters:**
- `filePath`: The path of a file or directory.

**Returns:**
- Directory information for `filePath`, or an empty string if `filePath` denotes a root directory or is null.

**Usage:**
```cpp
String dir = Path::GetDirectoryName(_T("C:\\Temp\\test.txt")); // "C:\\Temp"
```

##### `static String GetFileName(const String &filePath)`

Returns the file name and extension of the specified path string.

**Parameters:**
- `filePath`: The path string from which to obtain the file name and extension.

**Returns:**
- The characters after the last directory separator in `filePath`.

**Usage:**
```cpp
String filename = Path::GetFileName(_T("C:\\Temp\\test.txt")); // "test.txt"
```

##### `static String GetExtension(const String &filePath)`

Returns the extension of the specified path string.

**Parameters:**
- `filePath`: The path string from which to get the extension.

**Returns:**
- The extension of the specified path (including the period "."), or an empty string.

**Usage:**
```cpp
String ext = Path::GetExtension(_T("test.txt")); // ".txt"
```

##### `static String GetFileNameWithoutExtension(const String &filePath)`

Returns the file name of the specified path string without the extension.

**Parameters:**
- `filePath`: The path of the file.

**Returns:**
- The string returned by `GetFileName`, minus the last period and all characters following it.

**Usage:**
```cpp
String name = Path::GetFileNameWithoutExtension(_T("test.txt")); // "test"
```

##### `static String GetFullPath(const String &path)`

Returns the absolute path for the specified path string.

**Parameters:**
- `path`: The file or directory for which to obtain absolute path information.

**Returns:**
- The fully qualified location of `path`.

**Usage:**
```cpp
String fullPath = Path::GetFullPath(_T("test.txt"));
```

##### `static std::vector<TCHAR> GetInvalidFileNameChars()`

Gets an array of characters that are not allowed in file names.

**Returns:**
- A vector containing the characters that are not allowed in file names.

**Usage:**
```cpp
auto invalid = Path::GetInvalidFileNameChars();
```

##### `static std::vector<TCHAR> GetInvalidPathChars()`

Gets an array of characters that are not allowed in path names.

**Returns:**
- A vector containing the characters that are not allowed in path names.

**Usage:**
```cpp
auto invalid = Path::GetInvalidPathChars();
```
