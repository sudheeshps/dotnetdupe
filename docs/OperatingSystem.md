### class `OperatingSystem`

Represents information about an operating system, such as the version and platform identifier.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/OperatingSystem.h"
#include "../DotNetDupe/Environment.h"

int main() {
    DotNetDupe::System::OperatingSystem os = DotNetDupe::System::Environment::GetOperatingSystem();
    std::wcout << L"OS Platform: " << (int)os.GetPlatform() << std::endl;
    std::wcout << L"OS Version: " << os.GetVersionString().GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `OperatingSystem(PlatformID platform, const Version& version)`

Initializes a new instance of the `OperatingSystem` class, using the specified platform identifier and version object.

##### `PlatformID GetPlatform() const`

Gets a `PlatformID` enumeration value that identifies the operating system platform.

##### `const Version& GetVersion() const`

Gets a `Version` object that identifies the operating system.

##### `String GetVersionString() const`

Gets the concatenated string representation of the platform identifier, version, and service pack of the current operating system.

##### `String GetServicePack() const`

Gets the service pack version of the operating system.

##### `static bool IsWindows()`

Indicates whether the current operating system is a Windows operating system.

##### `static bool IsLinux()`

Indicates whether the current operating system is a Linux operating system.

##### `static bool IsMacOS()`

Indicates whether the current operating system is a macOS operating system.
