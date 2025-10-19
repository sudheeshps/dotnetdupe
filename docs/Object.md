### class `Object`

Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. This is the ultimate base class of all classes in the .NET Framework; it is the root of the type hierarchy.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Object.h"

int main() {
    DotNetDupe::System::Object obj1;
    DotNetDupe::System::Object obj2;

    std::cout << "obj1.Equals(obj2): " << (obj1.Equals(obj2) ? "true" : "false") << std::endl;
    
    return 0;
}
```

#### Methods

##### `bool operator == (const Object& obj) const`

Determines whether the specified `Object` is equal to the current `Object`.

##### `virtual bool Equals(const Object& obj)`

Determines whether the specified `Object` is equal to the current `Object`.

##### `static bool Equals(const Object& obj1, const Object& obj2)`

Determines whether the specified `Object` instances are considered equal.

##### `int GetHashCode()`

Serves as the default hash function.

##### `wchar_t* ToStringW()`

Returns a string that represents the current object (wide character version).

##### `char* ToStringA()`

Returns a string that represents the current object (ASCII character version).
