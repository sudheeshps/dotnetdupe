# OperatingSystem

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/OperatingSystem.h"`

Represents information about an operating system, such as the version and platform identifier.

---

## Syntax

```cpp
enum class PlatformID {
    Win32S = 0,
    Win32Windows = 1,
    Win32NT = 2,
    WinCE = 3,
    Unix = 4,
    Xbox = 5
};

class OperatingSystem : public Object;
```

---

## Constructors

### `OperatingSystem(PlatformID platform, const Version& version)`
Initializes a new instance of the `OperatingSystem` class using the specified platform identifier and version object.

---

## Static Methods

### `static bool IsWindows()`
Indicates whether the current application is running on Windows.

```cpp
if (OperatingSystem::IsWindows()) {
    // Windows specific logic
}
```

### `static bool IsLinux()`
Indicates whether the current application is running on Linux.

---

## Properties & Methods

### `PlatformID GetPlatform() const`
Gets a `PlatformID` enumeration value that identifies the operating system platform.

### `const Version& GetVersion() const`
Gets a `Version` object that identifies the operating system.

### `String GetVersionString() const`
Gets the concatenated string representation of the platform identifier, version, and service pack that are currently installed on the operating system.

### `String GetServicePack() const`
Gets the service pack version represented by this `OperatingSystem` object.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Environment.h"
#include "System/OperatingSystem.h"

using namespace DotNetDupe::System;

int main() {
    OperatingSystem objOS = Environment::GetOperatingSystem();

    Console::WriteLine("OS Version String: {0}", objOS.GetVersionString());
    Console::WriteLine("Service Pack:      {0}", objOS.GetServicePack());
    Console::WriteLine("Is Windows:        {0}", OperatingSystem::IsWindows());
    Console::WriteLine("Is Linux:          {0}", OperatingSystem::IsLinux());

    return 0;
}
```
