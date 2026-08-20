# Process & ProcessStartInfo

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/Process.h"`

Provides access to local processes, process querying and discovery, and starting/stopping system processes.

---

## `ProcessStartInfo`

Specifies a set of values that are used when you start a process.

### Syntax
```cpp
class ProcessStartInfo : public Object;
```

### Constructors
- `ProcessStartInfo()`: Initializes an empty `ProcessStartInfo`.
- `ProcessStartInfo(const String& sFileName)`: Initializes a `ProcessStartInfo` specifying the file name/executable.
- `ProcessStartInfo(const String& sFileName, const String& sArguments)`: Initializes a `ProcessStartInfo` specifying executable and command line arguments.

### Fields
- `String FileName`: The application or document to start.
- `String Arguments`: The set of command-line arguments to use when starting the application.
- `String WorkingDirectory`: The working directory for the process to be started.
- `bool CreateNoWindow`: Whether to start the process in a new window.
- `bool UseShellExecute`: Whether to use the operating system shell to start the process.

---

## `Process`

### Syntax
```cpp
class Process : public Object;
```

---

## Member Functions

### `ProcessStartInfo GetStartInfo() const` / `void SetStartInfo(const ProcessStartInfo& objStartInfo)`
Gets or sets the properties to pass to the `Start()` method.

### `bool Start()`
Starts the process resource that is specified by the `StartInfo` property.

### `void WaitForExit()`
Instructs the `Process` component to wait indefinitely for the associated process to exit.

### `bool WaitForExit(int iMilliseconds)`
Instructs the `Process` component to wait the specified number of milliseconds for the associated process to exit.
- **Returns:** `bool`: `true` if the associated process has exited; otherwise, `false`.

### `int GetExitCode() const`
Gets the value that the associated process specified when it terminated.

### `bool GetHasExited() const`
Gets a value indicating whether the associated process has been terminated.

### `int GetId() const`
Gets the unique native identifier (PID) for the associated process.

### `String GetProcessName() const`
Gets the name of the process.

### `void Kill()`
Immediately stops the associated process.

---

## Static Methods

### `static SmartPointer<Process> Start(const String& sFileName)`
Starts a process resource by specifying the name of an application.

### `static SmartPointer<Process> Start(const String& sFileName, const String& sArguments)`
Starts a process resource by specifying the executable file and command-line arguments.

### `static SmartPointer<Process> Start(const ProcessStartInfo& objStartInfo)`
Starts the process resource that is specified by the parameter containing process start information.

### `static int GetCurrentProcessId()`
Returns the process identifier of the calling process.

### `static SmartPointer<Process> GetCurrentProcess()`
Gets a new `Process` component and associates it with the currently active process.

### `static Array<SmartPointer<Process>> GetProcesses()`
Creates an array of new `Process` components and associates them with all active process resources on the system.

### `static SmartPointer<Process> GetProcessById(int iProcessId)`
Returns a new `Process` component, given the identifier of a process on the local machine. Throws `ArgumentException` if the process is not running.

### `static Array<SmartPointer<Process>> GetProcessesByName(const String& sProcessName)`
Creates an array of new `Process` components and associates them with all active processes sharing the specified process name.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/Process.h"
#include "System/SmartPointer.h"
#include "System/Array.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    // 1. Current Process Inspection
    auto spSelf = Process::GetCurrentProcess();
    Console::WriteLine("Current PID: {0} ({1})", spSelf->GetId(), spSelf->GetProcessName());

    // 2. Enumerate Running Processes
    auto arrProcesses = Process::GetProcesses();
    Console::WriteLine("Total Running Processes: {0}", arrProcesses.GetLength());

    // 3. Find Processes by Name
    auto arrSvchost = Process::GetProcessesByName("svchost");
    Console::WriteLine("Instances of svchost: {0}", arrSvchost.GetLength());

    // 4. Start Child Process
    ProcessStartInfo psi;
#if defined(_WIN32)
    psi.FileName = "cmd.exe";
    psi.Arguments = "/c echo Hello from child process";
#else
    psi.FileName = "/bin/sh";
    psi.Arguments = "-c \"echo Hello from child process\"";
#endif
    psi.CreateNoWindow = true;

    auto spChild = Process::Start(psi);
    spChild->WaitForExit(3000);

    Console::WriteLine("Child PID: {0}, Exited: {1}, ExitCode: {2}", 
        spChild->GetId(), spChild->GetHasExited(), spChild->GetExitCode());

    return 0;
}
```
