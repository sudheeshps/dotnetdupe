# Core Interfaces

**Namespace:** `DotNetDupe::System` & `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/IDisposable.h"`, `#include "System/IComparable.h"`

Core contract interfaces representing common object behaviors like resource cleanup, cloning, ordering comparisons, and custom formatting.

---

## `IDisposable`

**Namespace:** `DotNetDupe::System::IO`  
**Header:** `#include "System/IO/IDisposable.h"`

Defines a mechanism for releasing unmanaged resources deterministically.

### Syntax
```cpp
class IDisposable {
public:
    virtual ~IDisposable() = default;
    virtual void Dispose() = 0;
};
```

### Usage
```cpp
class DatabaseResource : public IDisposable {
public:
    void Dispose() override {
        // Free native database handles
    }
};
```

---

## `IComparable<T>`

**Namespace:** `DotNetDupe::System`

Defines a strongly-typed generalized comparison method that a value type or class implements to create a type-specific comparison method for ordering or sorting.

### Syntax
```cpp
template <typename T>
class IComparable {
public:
    virtual ~IComparable() = default;
    virtual int CompareTo(const T& other) const = 0;
};
```

### Return Values
- Less than zero: This instance precedes `other`.
- Zero: This instance occurs in the same position as `other`.
- Greater than zero: This instance follows `other`.

### Usage
```cpp
class Student : public IComparable<Student> {
public:
    int Score;

    int CompareTo(const Student& other) const override {
        return this->Score - other.Score;
    }
};
```

---

## `IFormatProvider<T>`

**Namespace:** `DotNetDupe::System`

Provides a mechanism for retrieving an object to control formatting.

### Syntax
```cpp
template <typename T>
class IFormatProvider {
public:
    virtual ~IFormatProvider() = default;
    virtual String GetFormat(const T& type) = 0;
};
```
