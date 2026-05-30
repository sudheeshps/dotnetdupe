### class `Environment`

Provides information about the current environment and platform.

#### Methods

##### `static String GetMachineName()`

Gets the NetBIOS name of this local computer.

**Returns:**
- A string containing the name of this computer.

**Usage:**
```cpp
String sMachineName = Environment::GetMachineName();
```

##### `static String GetUserName()`

Gets the user name of the person who is currently logged on to the Windows operating system.

**Returns:**
- The user name of the person who is logged on to Windows.

**Usage:**
```cpp
String sUserName = Environment::GetUserName();
```

##### `static int GetProcessorCount()`

Gets the number of processors on the current machine.

**Returns:**
- The number of processors on the current machine.

**Usage:**
```cpp
int nProcessors = Environment::GetProcessorCount();
```

##### `static String GetNewLine()`

Gets the newline string defined for this environment.

**Returns:**
- A string containing the newline characters.

**Usage:**
```cpp
String sNewLine = Environment::GetNewLine();
```

##### `static String GetCurrentDirectory()`

Gets the fully qualified path of the current working directory.

**Returns:**
- A string containing a directory path.

**Usage:**
```cpp
String sCurrentDir = Environment::GetCurrentDirectory();
```

##### `static String GetSystemDirectory()`

Gets the fully qualified path of the system directory.

**Returns:**
- A string containing a directory path.

**Usage:**
```cpp
String sSystemDir = Environment::GetSystemDirectory();
```

##### `static String GetOSVersion()`

Gets a string representation of the current platform identifier and version number.

**Returns:**
- A string that contains the OS version.

**Usage:**
```cpp
String sOSVersion = Environment::GetOSVersion();
```

##### `static String GetUserDomainName()`

Gets the user domain name of the person who is currently logged on to the Windows operating system.

**Returns:**
- The user domain name associated with the current user.

**Usage:**
```cpp
String sDomainName = Environment::GetUserDomainName();
```

##### `static int64_t GetWorkingSet()`

Gets the amount of physical memory mapped to the process context.

**Returns:**
- A 64-bit signed integer that contains the number of bytes of physical memory mapped to the process context.

**Usage:**
```cpp
int64_t nMemory = Environment::GetWorkingSet();
```

##### `static void Exit(int nExitCode)`

Terminates this process and gives the underlying operating system the specified exit code.

**Parameters:**
- `nExitCode`: Exit code to be given to the operating system.

**Usage:**
```cpp
Environment::Exit(0);
```

##### `static String ExpandEnvironmentVariables(const String& sName)`

Replaces the name of each environment variable embedded in the specified string with the string equivalent of the value of the variable.

**Parameters:**
- `sName`: A string containing the names of environment variables.

**Returns:**
- A string with each environment variable replaced by its value.

**Usage:**
```cpp
String sPath = Environment::ExpandEnvironmentVariables("%PATH%");
```

##### `static Array<String> GetCommandLineArgs()`

Returns a string array containing the command-line arguments for the current process.

**Returns:**
- An `Array<String>` where each element contains a command-line argument.

**Usage:**
```cpp
auto arrArgs = Environment::GetCommandLineArgs();
```

##### `static String GetEnvironmentVariable(const String& sVariable)`

Retrieves the value of an environment variable from the current process.

**Parameters:**
- `sVariable`: The name of an environment variable.

**Returns:**
- The value of the environment variable specified by `sVariable`, or an empty string if the environment variable is not found.

**Usage:**
```cpp
String sTemp = Environment::GetEnvironmentVariable("TEMP");
```

##### `static Collections::Generic::Dictionary<String, String> GetEnvironmentVariables()`

Retrieves all environment variable names and their values from the current process.

**Returns:**
- A `Dictionary` containing all environment variable names and their values.

**Usage:**
```cpp
auto dictEnvVars = Environment::GetEnvironmentVariables();
```

##### `static String GetFolderPath(SpecialFolder sfFolder)`

Gets the path to the system special folder that is identified by the specified enumeration.

**Parameters:**
- `sfFolder`: An enumerated constant that identifies a system special folder.

**Returns:**
- The path to the specified system special folder, if that folder physically exists on your computer; otherwise, an empty string.

**Usage:**
```cpp
String sDesktop = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
```

##### `static Array<String> GetLogicalDrives()`

Returns an array of strings that contains the names of the logical drives on the current computer.

**Returns:**
- An `Array<String>` where each element contains the name of a logical drive.

**Usage:**
```cpp
auto arrDrives = Environment::GetLogicalDrives();
```

##### `static void SetEnvironmentVariable(const String& sVariable, const String& sValue)`

Creates, modifies, or deletes an environment variable stored in the current process.

**Parameters:**
- `sVariable`: The name of an environment variable.
- `sValue`: A value to assign to `sVariable`.

**Usage:**
```cpp
Environment::SetEnvironmentVariable("MYVAR", "MYVALUE");
```

##### `static OperatingSystem GetOperatingSystem()`

Gets an `OperatingSystem` object that contains the current platform identifier and version number.

**Returns:**
- An `OperatingSystem` object.

**Usage:**
```cpp
OperatingSystem osCurrent = Environment::GetOperatingSystem();
```

