# Version

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Version.h"`

Represents the version number of an assembly, operating system, or library (`major.minor.build.revision`).

---

## Syntax

```cpp
class Version : public Object;
```

---

## Constructors

### `Version()`
Initializes a new default `Version` instance (`0.0.0.0`).

### `Version(int iMajor, int iMinor)`
Initializes a new `Version` instance with major and minor components.

### `Version(int iMajor, int iMinor, int iBuild)`
Initializes a new `Version` instance with major, minor, and build components.

### `Version(int iMajor, int iMinor, int iBuild, int iRevision)`
Initializes a new `Version` instance with major, minor, build, and revision components.

```cpp
Version ver(1, 4, 0, 102);
```

---

## Properties & Methods

- `int GetMajor() const`: Gets major version number.
- `int GetMinor() const`: Gets minor version number.
- `int GetBuild() const`: Gets build version number.
- `int GetRevision() const`: Gets revision version number.

### `String ToString() const`
Converts the value of the current `Version` object to its equivalent `String` representation (`"1.4.0.102"`).

### `bool operator==(const Version& vOther) const`
Determines whether two `Version` instances represent the same version value.

### `bool operator!=(const Version& vOther) const`
Determines whether two `Version` instances represent different version values.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Version.h"

using namespace DotNetDupe::System;

int main() {
    Version verCurrent(2, 0, 1);
    Console::WriteLine("Current Version: {0}", verCurrent.ToString());
    Console::WriteLine("Major: {0}, Minor: {1}", verCurrent.GetMajor(), verCurrent.GetMinor());

    return 0;
}
```
