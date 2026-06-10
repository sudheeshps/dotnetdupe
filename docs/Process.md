### class `Process`

Provides access to local processes and enables you to start and stop local system processes.

#### Methods

##### `static SmartPointer<Process> Start(const String& sFileName)`

Starts a process resource by specifying the name of an application.

**Usage:**
```cpp
auto pProcess = Process::Start("notepad.exe");
```

##### `static SmartPointer<Process> Start(const String& sFileName, const String& sArguments)`

Starts a process resource by specifying the name of an application and a set of command-line arguments.

**Usage:**
```cpp
auto pProcess = Process::Start("cmd.exe", "/c dir");
```

##### `static SmartPointer<Process> Start(const ProcessStartInfo& objStartInfo)`

Starts a process resource by specifying a `ProcessStartInfo` object.

**Usage:**
```cpp
ProcessStartInfo objInfo("cmd.exe", "/c dir");
objInfo.CreateNoWindow = true;
auto pProcess = Process::Start(objInfo);
```

##### `bool Start()`

Starts the process resource that is specified by the `StartInfo` property.

**Usage:**
```cpp
Process objProcess;
ProcessStartInfo objInfo("notepad.exe");
objProcess.SetStartInfo(objInfo);
objProcess.Start();
```

##### `void WaitForExit()`

Instructs the `Process` component to wait indefinitely for the associated process to exit.

**Usage:**
```cpp
pProcess->WaitForExit();
```

##### `bool WaitForExit(int iMilliseconds)`

Instructs the `Process` component to wait the specified number of milliseconds for the associated process to exit.

**Usage:**
```cpp
bool bExited = pProcess->WaitForExit(5000);
```

##### `void Kill()`

Immediately stops the associated process.

**Usage:**
```cpp
pProcess->Kill();
```

##### `int GetExitCode() const`

Gets the value that the associated process specified when it terminated.

**Usage:**
```cpp
int iCode = pProcess->GetExitCode();
```

##### `bool GetHasExited()`

Gets a value indicating whether the associated process has been terminated.

**Usage:**
```cpp
if (pProcess->GetHasExited()) { /* ... */ }
```

##### `int GetId() const`

Gets the unique identifier for the associated process.

**Usage:**
```cpp
int iId = pProcess->GetId();
```

#### class `ProcessStartInfo`

Specifies a set of values that are used when you start a process.

##### Properties

- `String FileName`: The application or document to start.
- `String Arguments`: The set of command-line arguments to use when starting the application.
- `String WorkingDirectory`: The working directory for the process to be started.
- `bool CreateNoWindow`: Whether to start the process in a new window.
- `bool UseShellExecute`: Whether to use the operating system shell to start the process.
