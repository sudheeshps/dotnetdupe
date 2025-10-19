### interface `IClonable`

Defines a general-purpose mechanism for creating a new object that is a copy of the current object.

#### Methods

##### `virtual Object Clone() = 0`

Creates a new object that is a copy of the current instance.

---

### interface `IComparable`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Comparable.h"
#include "../DotNetDupe/Object.h"

class MyComparable : public DotNetDupe::System::Object, public DotNetDupe::System::IComparable {
public:
    int value;
    MyComparable(int val) : value(val) {}

    int CompareTo(const Object& obj) override {
        const MyComparable& other = static_cast<const MyComparable&>(obj);
        if (value < other.value) return -1;
        if (value > other.value) return 1;
        return 0;
    }
};

int main() {
    MyComparable a(1);
    MyComparable b(2);
    std::cout << a.CompareTo(b) << std::endl;
    return 0;
}
```

#### Methods

##### `virtual int CompareTo(const Object& obj) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

---

### template interface `IComparable<T>`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Methods

##### `virtual int CompareTo<T>(const T& other) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

---

### interface `IFormatProvider<T>`

Provides a mechanism for retrieving a formatting service for a specified type.

#### Methods

##### `virtual Object* GetFormat(const T* formatType) = 0`

Returns an object that provides formatting services for the specified type.
