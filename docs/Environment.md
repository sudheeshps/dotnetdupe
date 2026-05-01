### class `Environment`

Provides information about the current environment and platform.

#### Methods

##### `static String GetMachineName()`

Gets the NetBIOS name of this local computer.

**Returns:**
- A string containing the name of this computer.

**Usage:**
```cpp
String machineName = Environment::GetMachineName();
```

##### `static String GetUserName()`

Gets the user name of the person who is currently logged on to the Windows operating system.

**Returns:**
- The user name of the person who is logged on to Windows.

**Usage:**
```cpp
String userName = Environment::GetUserName();
```

##### `static int GetProcessorCount()`

Gets the number of processors on the current machine.

**Returns:**
- The number of processors on the current machine.

**Usage:**
```cpp
int processors = Environment::GetProcessorCount();
```

##### `static String GetNewLine()`

Gets the newline string defined for this environment.

**Returns:**
- A string containing the newline characters.

**Usage:**
```cpp
String newLine = Environment::GetNewLine();
```

##### `static String GetCurrentDirectory()`

Gets the fully qualified path of the current working directory.

**Returns:**
- A string containing a directory path.

**Usage:**
```cpp
String currentDir = Environment::GetCurrentDirectory();
```

##### `static String GetSystemDirectory()`

Gets the fully qualified path of the system directory.

**Returns:**
- A string containing a directory path.

**Usage:**
```cpp
String systemDir = Environment::GetSystemDirectory();
```

##### `static String GetOSVersion()`

Gets a string representation of the current platform identifier and version number.

**Returns:**
- A string that contains the OS version.

**Usage:**
```cpp
String osVersion = Environment::GetOSVersion();
```

##### `static String GetUserDomainName()`

Gets the user domain name of the person who is currently logged on to the Windows operating system.

**Returns:**
- The user domain name associated with the current user.

**Usage:**
```cpp
String domainName = Environment::GetUserDomainName();
```

##### `static String GetVersion()`

Gets a string representation of the version numbers of the common language runtime.

**Returns:**
- A string that describes the version.

**Usage:**
```cpp
String version = Environment::GetVersion();
```

##### `static int64_t GetWorkingSet()`

Gets the amount of physical memory mapped to the process context.

**Returns:**
- A 64-bit signed integer that contains the number of bytes of physical memory mapped to the process context.

**Usage:**
```cpp
int64_t memory = Environment::GetWorkingSet();
```

##### `static void Exit(int exitCode)`

Terminates this process and gives the underlying operating system the specified exit code.

**Parameters:**
- `exitCode`: Exit code to be given to the operating system.

**Usage:**
```cpp
Environment::Exit(0);
```

##### `static String ExpandEnvironmentVariables(const String& name)`

Replaces the name of each environment variable embedded in the specified string with the string equivalent of the value of the variable.

**Parameters:**
- `name`: A string containing the names of environment variables.

**Returns:**
- A string with each environment variable replaced by its value.

**Usage:**
```cpp
String path = Environment::ExpandEnvironmentVariables(_T("%PATH%"));
```

##### `static std::vector<String> GetCommandLineArgs()`

Returns a string array containing the command-line arguments for the current process.

**Returns:**
- A vector of strings where each element contains a command-line argument.

**Usage:**
```cpp
auto args = Environment::GetCommandLineArgs();
```

##### `static String GetEnvironmentVariable(const String& variable)`

Retrieves the value of an environment variable from the current process.

**Parameters:**
- `variable`: The name of an environment variable.

**Returns:**
- The value of the environment variable specified by `variable`, or an empty string if the environment variable is not found.

**Usage:**
```cpp
String temp = Environment::GetEnvironmentVariable(_T("TEMP"));
```

##### `static std::map<String, String> GetEnvironmentVariables()`

Retrieves all environment variable names and their values from the current process.

**Returns:**
- A map containing all environment variable names and their values.

**Usage:**
```cpp
auto envVars = Environment::GetEnvironmentVariables();
```

##### `static String GetFolderPath(SpecialFolder folder)`

Gets the path to the system special folder that is identified by the specified enumeration.

**Parameters:**
- `folder`: An enumerated constant that identifies a system special folder.

**Returns:**
- The path to the specified system special folder, if that folder physically exists on your computer; otherwise, an empty string.

**Usage:**
```cpp
String desktop = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
```

##### `static std::vector<String> GetLogicalDrives()`

Returns an array of strings that contains the names of the logical drives on the current computer.

**Returns:**
- A vector of strings where each element contains the name of a logical drive.

**Usage:**
```cpp
auto drives = Environment::GetLogicalDrives();
```

##### `static void SetEnvironmentVariable(const String& variable, const String& value)`

Creates, modifies, or deletes an environment variable stored in the current process.

**Parameters:**
- `variable`: The name of an environment variable.
- `value`: A value to assign to `variable`.

**Usage:**
```cpp
Environment::SetEnvironmentVariable(_T("MYVAR"), _T("MYVALUE"));
```

##### `static OperatingSystem GetOperatingSystem()`

Gets an `OperatingSystem` object that contains the current platform identifier and version number.

**Returns:**
- An `OperatingSystem` object.

**Usage:**
```cpp
OperatingSystem os = Environment::GetOperatingSystem();
```
