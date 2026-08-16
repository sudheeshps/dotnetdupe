# Core Interfaces

**Namespace:** `DotNetDupe::System` & `DotNetDupe::System::IO`  
**Headers:** `#include "System/IO/IDisposable.h"`, `#include "System/Comparable.h"`, `#include "System/Clonable.h"`, `#include "System/FormatProvider.h"`, `#include "System/IServiceProvider.h"`

Core contract interfaces representing common object behaviors like resource cleanup, cloning, ordering comparisons, custom formatting, and service resolution.

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

## `IComparable`

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Comparable.h"`

Defines a generalized comparison method that a value type or class implements to create a type-specific comparison method for ordering or sorting.

### Syntax
```cpp
class IComparable {
public:
    virtual ~IComparable() = default;
    virtual int CompareTo(const Object& obj) = 0;
};
```

---

## `IClonable`

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Clonable.h"`

Supports cloning, which creates a new instance of a class with the same value as an existing instance.

### Syntax
```cpp
class IClonable {
public:
    virtual ~IClonable() = default;
    virtual Object Clone() = 0;
};
```

---

## `IFormatProvider<T>`

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/FormatProvider.h"`

Provides a mechanism for retrieving an object to control formatting.

### Syntax
```cpp
template <class T>
class IFormatProvider {
public:
    virtual ~IFormatProvider() = default;
    virtual Object* GetFormat(const T* formatType) = 0;
};
```

---

## `IServiceProvider`

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/IServiceProvider.h"`

Defines a mechanism for retrieving a service object; that is, an object that provides custom support to other objects.

### Syntax
```cpp
class IServiceProvider : public Object {
public:
    virtual ~IServiceProvider() = default;
    virtual SmartPointer<Object> GetService(const std::type_index& serviceType) = 0;

    template <typename T>
    SmartPointer<T> GetService();

    template <typename T>
    SmartPointer<T> GetRequiredService();
};
```
