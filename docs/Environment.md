# Environment

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Environment.h"`

Provides cross-platform information about the current execution environment, machine properties, user session, logical drives, special folders, command-line arguments, variable expansion, and environment variables.

---

## Syntax

```cpp
enum class SpecialFolder {
    ApplicationData,
    CommonApplicationData,
    CommonProgramFiles,
    Cookies,
    Desktop,
    Favorites,
    History,
    InternetCache,
    LocalApplicationData,
    MyComputer,
    MyDocuments,
    MyMusic,
    MyPictures,
    MyVideos,
    ProgramFiles,
    Programs,
    Recent,
    SendTo,
    StartMenu,
    Startup,
    System,
    Templates,
    UserProfile
};

class Environment : public Object;
```

---

## Static Properties & Methods

### `static String GetMachineName()`
Gets the NetBIOS / hostname of the local computer.

### `static String GetUserName()`
Gets the user name of the person who is currently logged on to the operating system.

### `static String GetUserDomainName()`
Gets the network domain name associated with the current user.

### `static int GetProcessorCount()`
Gets the number of logical processors on the current machine.

### `static String GetNewLine()`
Gets the newline string defined for this environment (`"\r\n"` on Windows, `"\n"` on POSIX).

### `static String GetCurrentDirectory()`
Gets the fully qualified path of the current working directory.

### `static String GetSystemDirectory()`
Gets the fully qualified path of the system directory (e.g. `C:\Windows\System32`).

### `static String GetOSVersion()`
Gets the version and description of the current operating system.

### `static OperatingSystem GetOperatingSystem()`
Returns an `OperatingSystem` object containing platform and version metadata.

### `static Array<String> GetCommandLineArgs()`
Returns an array of strings containing the command-line arguments for the current process.

```cpp
Array<String> arrArgs = Environment::GetCommandLineArgs();
for (int iIdx = 0; iIdx < arrArgs.GetLength(); ++iIdx) {
    Console::WriteLine("Arg[{0}]: {1}", iIdx, arrArgs[iIdx]);
}
```

### `static String ExpandEnvironmentVariables(const String& sName)`
Replaces the name of each environment variable embedded in the specified string with the string equivalent of the value of the variable (e.g. `"%PATH%;%TEMP%"` or `"$HOME"`).

```cpp
String sExpanded = Environment::ExpandEnvironmentVariables("%TEMP%\\myapp.log");
```

### `static String GetEnvironmentVariable(const String& sVariable)`
Retrieves the value of an environment variable from the current process.

### `static void SetEnvironmentVariable(const String& sVariable, const String& sValue)`
Creates, modifies, or deletes an environment variable stored in the current process.

### `static Collections::Generic::Dictionary<String, String> GetEnvironmentVariables()`
Retrieves all environment variable names and their values from the current process as a key-value dictionary.

### `static String GetFolderPath(SpecialFolder eFolder)`
Gets the path to the system special folder identified by the specified enumeration.

### `static Array<String> GetLogicalDrives()`
Returns an array of strings containing the names of the logical drives on the current computer.

### `static int64_t GetWorkingSet()`
Gets the amount of physical memory mapped to the process context.

### `static void Exit(int iExitCode)`
Terminates the current process and returns an exit code to the operating system.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Environment.h"

using namespace DotNetDupe::System;

int main() {
    Console::WriteLine("Machine Name:   {0}", Environment::GetMachineName());
    Console::WriteLine("User Name:      {0}", Environment::GetUserName());
    Console::WriteLine("Domain:         {0}", Environment::GetUserDomainName());
    Console::WriteLine("CPU Cores:      {0}", Environment::GetProcessorCount());
    Console::WriteLine("Working Set:    {0} bytes", Environment::GetWorkingSet());
    Console::WriteLine("System Folder:  {0}", Environment::GetSystemDirectory());
    Console::WriteLine("Expanded Temp:  {0}", Environment::ExpandEnvironmentVariables("%TEMP%"));

    return 0;
}
```
