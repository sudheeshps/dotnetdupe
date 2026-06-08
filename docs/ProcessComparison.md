# Process Management Comparison: STL vs. DotNetDupe

This guide provides a side-by-side comparison of process management tasks using standard C++ (traditional C-style APIs) and DotNetDupe.

---

## 1. Process Creation and Management

Standard C++ (up to C++23) does not have a built-in `std::process` class. Developers typically rely on platform-specific C APIs or external libraries like Boost.Process.

| Feature | Standard C++ (Traditional / Platform APIs) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | Windows: `<windows.h>`<br>Linux: `<unistd.h>`, `<sys/wait.h>` | `#include "System/Diagnostics/Process.h"` |
| **Basic Execution** | `system("command args");` | `Process::Start("command", "args");` |
| **Advanced Start** | **Windows:** `CreateProcessW(...)`<br>**Linux:** `fork()` + `execvp()` | `ProcessStartInfo info("app", "args");`<br>`info.CreateNoWindow = true;`<br>`Process::Start(info);` |
| **Wait for Exit** | **Windows:** `WaitForSingleObject(hProcess, ...)`<br>**Linux:** `waitpid(pid, ...)` | `p->WaitForExit();` |
| **Get Exit Code** | **Windows:** `GetExitCodeProcess(...)`<br>**Linux:** `WEXITSTATUS(status)` | `int code = p->GetExitCode();` |
| **Kill Process** | **Windows:** `TerminateProcess(...)`<br>**Linux:** `kill(pid, SIGKILL)` | `p->Kill();` |

---

## 2. Comparison Examples

### Example: Running a command and waiting for it to finish

#### Windows (Win32 API)
```cpp
#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring cmd = L"cmd.exe /c echo Hello";

    if (CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        std::cout << "Exited with: " << exitCode << std::endl;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    return 0;
}
```

#### Linux (POSIX)
```cpp
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child
        char* argv[] = {(char*)"/bin/sh", (char*)"-c", (char*)"echo Hello", NULL};
        execvp(argv[0], argv);
        _exit(1);
    } else if (pid > 0) {
        // Parent
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "Exited with: " << WEXITSTATUS(status) << std::endl;
        }
    }
    return 0;
}
```

#### DotNetDupe (Cross-Platform)
```cpp
#include "System/Diagnostics/Process.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    auto p = Process::Start("cmd.exe", "/c echo Hello"); // or "/bin/sh" on Linux
    if (!p.IsNull()) {
        p->WaitForExit();
        Console::Write("Exited with: ");
        Console::WriteLine(p->GetExitCode());
    }
    return 0;
}
```

---

## 3. Advanced Process Configuration

DotNetDupe simplifies process configuration through `ProcessStartInfo`, avoiding the complex bitmasks and structures required by native APIs.

| Standard C++ (Platform APIs) | DotNetDupe |
| :--- | :--- |
| **Windows:**<br>`si.dwFlags = STARTF_USESHOWWINDOW;`<br>`si.wShowWindow = SW_HIDE;`<br>`CreateProcess(..., CREATE_NO_WINDOW, ...);` | `ProcessStartInfo info("app");`<br>`info.CreateNoWindow = true;`<br>`Process::Start(info);` |

---

## Conclusion

**DotNetDupe::Process** provides a unified, object-oriented way to manage external processes. It abstracts away the significant differences between the Win32 `CreateProcess` model and the POSIX `fork`/`exec` model, allowing developers to write portable code with minimal effort.
