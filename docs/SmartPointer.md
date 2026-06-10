### class `SmartPointer<T>`

A unified smart pointer that supports both **unique** and **shared** ownership semantics within a single interface. It provides a "managed" feel by automatically allocating objects and supporting C#-like instantiation patterns.

#### Constructors

##### `SmartPointer()`

Initializes a new `SmartPointer`.
- **For concrete types**: Automatically allocates a new instance of `T` using `new T()`. Unique ownership.
- **For abstract types**: Initializes to `nullptr`.

**Usage:**
```cpp
SmartPointer<MyClass> pObj; // Automatically calls new MyClass()
```

##### `explicit SmartPointer(bool bIsShared)`

Initializes a new `SmartPointer` and automatically allocates a new instance of `T`.
- If `bIsShared` is `true`: Enables **Shared** ownership (reference counting).
- If `bIsShared` is `false`: Enables **Unique** ownership.

**Usage:**
```cpp
SmartPointer<MyClass> pShared(true); // Automatically calls new MyClass() and enables sharing
```

##### `explicit SmartPointer(T* pPtr)`

Takes ownership of an existing raw pointer. Unique ownership.

##### `SmartPointer(T* pPtr, bool bIsShared)`

Takes ownership of an existing raw pointer with specified ownership mode.

##### `SmartPointer(std::nullptr_t)`

Explicitly initializes an empty (null) `SmartPointer`.

#### Static Helpers

##### `static SmartPointer<T> New(Args&&... args)`

Creates a new `SmartPointer` with unique ownership, passing the specified arguments to `T`'s constructor.

**Usage:**
```cpp
auto pObj = SmartPointer<MyClass>::New("arg1", 42);
```

##### `static SmartPointer<T> NewShared(Args&&... args)`

Creates a new `SmartPointer` with shared ownership, passing the specified arguments to `T`'s constructor.

**Usage:**
```cpp
auto pShared = SmartPointer<MyClass>::NewShared("arg1", 42);
```

#### Methods

##### `void Attach(T* pPtr, bool bIsShared = false)`

Takes ownership of a new raw pointer, cleaning up the previously managed object.

##### `T* Detach()`

Releases ownership of the managed object and returns the raw pointer. The `SmartPointer` becomes null.

##### `T* Get() const`

Returns the underlying raw pointer without releasing ownership.

##### `int GetRefCount() const`

Returns the current reference count. Returns `0` for Unique pointers or null pointers.

##### `bool IsNull() const`

Returns `true` if the managed pointer is `nullptr`.

#### Example

```cpp
void Demonstrate() {
    // 1. Auto-allocation (Unique)
    SmartPointer<String> pS;
    Console::WriteLine(pS->GetLength());

    // 2. Auto-allocation (Shared)
    SmartPointer<String> pShared(true);
    auto pCopy = pShared; // Ref count increments

    // 3. Static Factory (with constructor args)
    auto pVersion = SmartPointer<Version>::New(1, 0, 5);
    Console::WriteLine(pVersion->ToString());

    // 4. Abstract classes (must be manual or factory)
    SmartPointer<TextReader> pReader(new StringReader("data"));
}
```

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `SmartPointer`.

```cpp
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Console.h"
#include "System/Version.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;

int main() {
    try {
        // 1. Auto-allocated Unique ownership
        SmartPointer<String> pUniqueStr;
        *pUniqueStr = "Hello Unique";
        Console::WriteLine(*pUniqueStr);

        // 2. Auto-allocated Shared ownership (ref counted)
        SmartPointer<String> pSharedStr(true);
        *pSharedStr = "Hello Shared";
        {
            SmartPointer<String> pSharedCopy = pSharedStr; // Increments ref count
            Console::Write("Copy content: ");
            Console::WriteLine(*pSharedCopy);
            Console::Write("Ref Count: ");
            Console::WriteLine(pSharedStr.GetRefCount()); // Should be 2
        }
        Console::Write("Ref Count after copy went out of scope: ");
        Console::WriteLine(pSharedStr.GetRefCount()); // Should be 1

        // 3. Static Factory (with constructor args)
        auto pVersion = SmartPointer<Version>::New(2, 1, 0);
        Console::Write("Version via SmartPointer: ");
        Console::WriteLine(pVersion->ToString());

    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```

