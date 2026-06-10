### class `Version`

Represents the version number of an assembly, operating system, or the common language runtime.

#### Methods

##### `Version(int nMajor, int nMinor, int nBuild, int nRevision)`

Initializes a new instance of the `Version` class with the specified major, minor, build, and revision numbers.

**Parameters:**
- `nMajor`: The major version number.
- `nMinor`: The minor version number.
- `nBuild`: The build number.
- `nRevision`: The revision number.

**Usage:**
```cpp
Version vVersion(1, 2, 3, 4);
```

##### `Version(int nMajor, int nMinor, int nBuild)`

Initializes a new instance of the `Version` class with the specified major, minor, and build numbers.

**Parameters:**
- `nMajor`: The major version number.
- `nMinor`: The minor version number.
- `nBuild`: The build number.

**Usage:**
```cpp
Version vVersion(1, 2, 3);
```

##### `Version(int nMajor, int nMinor)`

Initializes a new instance of the `Version` class with the specified major and minor numbers.

**Parameters:**
- `nMajor`: The major version number.
- `nMinor`: The minor version number.

**Usage:**
```cpp
Version vVersion(1, 2);
```

##### `Version()`

Initializes a new instance of the `Version` class.

**Usage:**
```cpp
Version vVersion;
```

##### `int GetMajor() const`

Gets the value of the major component of the version number.

**Returns:**
- The major version number.

**Usage:**
```cpp
int nMajor = vVersion.GetMajor();
```

##### `int GetMinor() const`

Gets the value of the minor component of the version number.

**Returns:**
- The minor version number.

**Usage:**
```cpp
int nMinor = vVersion.GetMinor();
```

##### `int GetBuild() const`

Gets the value of the build component of the version number.

**Returns:**
- The build number, or -1 if the build number is undefined.

**Usage:**
```cpp
int nBuild = vVersion.GetBuild();
```

##### `int GetRevision() const`

Gets the value of the revision component of the version number.

**Returns:**
- The revision number, or -1 if the revision number is undefined.

**Usage:**
```cpp
int nRevision = vVersion.GetRevision();
```

##### `String ToString() const`

Converts the value of the current `Version` object to its equivalent `String` representation.

**Returns:**
- The `String` representation of the values of the major, minor, build, and revision components.

**Usage:**
```cpp
String sVersion = vVersion.ToString(); // "1.2.3.4"
```

##### `bool operator==(const Version& vOther) const`

Determines whether two specified `Version` objects are equal.

**Parameters:**
- `vOther`: The `Version` object to compare with the current object.

**Returns:**
- `true` if every component of the current `Version` object matches the corresponding component of the `vOther` parameter; otherwise, `false`.

**Usage:**
```cpp
Version vVer1(1, 0), vVer2(1, 0);
if (vVer1 == vVer2) { /* ... */ }
```

---

## Code Example

The following example demonstrates how to create `Version` instances using different constructors, inspect individual version components, check equality, and manage `Version` instances dynamically using `SmartPointer`.

```cpp
#include "System/Console.h"
#include "System/Version.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;

int main() {
    // Instantiate Version objects
    Version v1(1, 2, 3, 4);
    Version v2(1, 2);

    Console::Write("v1 string representation: ");
    Console::WriteLine(v1.ToString());

    Console::Write("v2 string representation: ");
    Console::WriteLine(v2.ToString());

    // Compare version components
    Console::Write("v1 Major: ");
    Console::WriteLine(v1.GetMajor());

    Console::Write("v1 Minor: ");
    Console::WriteLine(v1.GetMinor());

    Console::Write("v1 Build: ");
    Console::WriteLine(v1.GetBuild());

    Console::Write("v1 Revision: ");
    Console::WriteLine(v1.GetRevision());

    // Comparison operators
    if (v1 != v2) {
        Console::WriteLine("v1 is not equal to v2.");
    }

    // Dynamic memory management using SmartPointer
    auto pVer = SmartPointer<Version>::New(2, 0, 0);
    Console::Write("pVer string representation: ");
    Console::WriteLine(pVer->ToString());

    return 0;
}
```


