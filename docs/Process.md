# Process &amp; ProcessStartInfo

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/Process.h"`

Provides access to local and remote processes and enables you to start and stop local system processes.

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
- **Returns:**
  - `bool`: `true` if the associated process has exited; otherwise, `false`.

### `int GetExitCode() const`
Gets the value that the associated process specified when it terminated.

### `bool GetHasExited() const`
Gets a value indicating whether the associated process has been terminated.

### `int GetId() const`
Gets the unique native identifier (PID) for the associated process.

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

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/Process.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    Console::WriteLine("Current Process ID: {0}", Process::GetCurrentProcessId());

    ProcessStartInfo psi;
#if defined(_WIN32)
    psi.FileName = "cmd.exe";
    psi.Arguments = "/c echo Hello from child process";
#else
    psi.FileName = "/bin/sh";
    psi.Arguments = "-c \"echo Hello from child process\"";
#endif
    psi.CreateNoWindow = true;

    auto pProc = Process::Start(psi);
    pProc->WaitForExit(3000);

    Console::WriteLine("Child Process PID: {0}", pProc->GetId());
    Console::WriteLine("Has Exited:        {0}", pProc->GetHasExited());
    Console::WriteLine("Exit Code:         {0}", pProc->GetExitCode());

    return 0;
}
```
