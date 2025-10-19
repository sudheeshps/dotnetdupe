### class `Environment`

Provides information about the current environment and platform.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Environment.h"

int main() {
    std::wcout << L"Current Directory: " << DotNetDupe::System::Environment::GetCurrentDirectory().GetRawString() << std::endl;
    std::wcout << L"OS Version: " << DotNetDupe::System::Environment::GetOSVersion().GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `static String GetMachineName()`

Gets the NetBIOS name of this local computer.

##### `static String GetUserName()`

Gets the user name of the person who is currently logged on to the Windows operating system.

##### `static int GetProcessorCount()`

Gets the number of processors on the current machine.

##### `static String GetNewLine()`

Gets the newline string defined for this environment.

##### `static String GetCurrentDirectory()`

Gets the fully qualified path of the current working directory.

##### `static String GetSystemDirectory()`

Gets the fully qualified path of the system directory.

##### `static String GetOSVersion()`

Gets an `OperatingSystem` object that contains the current platform identifier and version number.

##### `static String GetUserDomainName()`

Gets the user domain name of the person who is currently logged on to the Windows operating system.

##### `static String GetVersion()`

Gets a `Version` object that describes the major, minor, build, and revision numbers of the common language runtime.

##### `static int64_t GetWorkingSet()`

Gets the amount of physical memory mapped to the process context.

##### `static void Exit(int exitCode)`

Terminates this process and gives the underlying operating system the specified exit code.

##### `static String ExpandEnvironmentVariables(const String& name)`

Replaces the name of each environment variable embedded in the specified string with the string equivalent of the value of the variable, then returns the resulting string.

##### `static std::vector<String> GetCommandLineArgs()`

Returns a string array containing the command-line arguments for the current process.

##### `static String GetEnvironmentVariable(const String& variable)`

Retrieves the value of an environment variable from the current process.

##### `static std::map<String, String> GetEnvironmentVariables()`

Retrieves all environment variable names and their values from the current process.

##### `static String GetFolderPath(SpecialFolder folder)`

Gets the path to the system special folder that is identified by the specified enumeration.

##### `static std::vector<String> GetLogicalDrives()`

Returns an array of strings that contains the names of the logical drives on the current computer.

##### `static void SetEnvironmentVariable(const String& variable, const String& value)`

Creates, modifies, or deletes an environment variable stored in the current process.

##### `static OperatingSystem GetOperatingSystem()`

Gets an `OperatingSystem` object that contains the current platform identifier and version number.
