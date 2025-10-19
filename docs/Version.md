### class `Version`

Represents the version number of an assembly, operating system, or the common language runtime.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Version.h"

int main() {
    DotNetDupe::System::Version version(1, 2, 3, 4);
    std::wcout << L"Version: " << version.ToString().GetRawString() << std::endl;
    return 0;
}
```

#### Methods

##### `Version(int major, int minor, int build, int revision)`

Initializes a new instance of the `Version` class with the specified major, minor, build, and revision numbers.

##### `Version(int major, int minor, int build)`

Initializes a new instance of the `Version` class with the specified major, minor, and build numbers.

##### `Version(int major, int minor)`

Initializes a new instance of the `Version` class with the specified major and minor numbers.

##### `Version()`

Initializes a new instance of the `Version` class.

##### `int GetMajor() const`

Gets the value of the major component of the version number for the current `Version` object.

##### `int GetMinor() const`

Gets the value of the minor component of the version number for the current `Version` object.

##### `int GetBuild() const`

Gets the value of the build component of the version number for the current `Version` object.

##### `int GetRevision() const`

Gets the value of the revision component of the version number for the current `Version` object.

##### `String ToString() const`

Converts the value of the current `Version` object to its equivalent `String` representation.

##### `bool operator==(const Version& other) const`

Determines whether two specified `Version` objects are equal.

##### `bool operator!=(const Version& other) const`

Determines whether two specified `Version` objects are not equal.
