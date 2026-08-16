# SmartPointer&lt;T&gt;

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/SmartPointer.h"`

A unified, exception-safe RAII smart pointer supporting both **Unique** and **Shared reference-counted** ownership semantics with zero unmanaged raw pointer leakages.

---

## Syntax

```cpp
template <typename T>
class SmartPointer;
```

---

## Static Factory Methods (Recommended)

### `static SmartPointer<T> NewUnique()` / `template <typename Arg1, typename... Args> static SmartPointer<T> NewUnique(Arg1&& arg1, Args&&... args)`
Creates and constructs a uniquely owned instance of `T` by forwarding arguments to `T`'s constructor.

```cpp
auto pBuffer = SmartPointer<MemoryStream>::NewUnique();
```

### `static SmartPointer<T> NewShared()` / `template <typename Arg1, typename... Args> static SmartPointer<T> NewShared(Arg1&& arg1, Args&&... args)`
Creates and constructs a shared reference-counted instance of `T` by forwarding arguments to `T`'s constructor.

```cpp
auto spFile = SmartPointer<FileStream>::NewShared("data.bin", FileMode::Open);
```

### `static SmartPointer<T> New()` / `template <typename Arg1, typename... Args> static SmartPointer<T> New(Arg1&& arg1, Args&&... args)`
Alias for `NewUnique()`, providing C#-like allocation syntax.

### `static SmartPointer<T> MakeUnique()` / `template <typename Arg1, typename... Args> static SmartPointer<T> MakeUnique(Arg1&& arg1, Args&&... args)`
Standard alias for `NewUnique()`.

### `static SmartPointer<T> MakeShared()` / `template <typename Arg1, typename... Args> static SmartPointer<T> MakeShared(Arg1&& arg1, Args&&... args)`
Standard alias for `NewShared()`.

---

## Constructors & Assignment

### `SmartPointer()`
Default constructor. For concrete types, automatically allocates `new T()` with unique ownership. For abstract classes, initializes to `nullptr`.

### `explicit SmartPointer(bool bIsShared)`
Constructor with ownership mode flag. Automatically allocates `new T()` and sets shared mode if `bIsShared` is `true`.

### `explicit SmartPointer(T* pPtr)`
Takes unique ownership of an existing raw pointer.

### `SmartPointer(T* pPtr, bool bIsShared)`
Takes ownership of an existing raw pointer with the specified ownership mode.

### `SmartPointer(std::nullptr_t)`
Explicit null constructor.

### `SmartPointer(const SmartPointer<T>& objOther)` / `template <typename U> SmartPointer(const SmartPointer<U>& objOther)`
Copy constructor. Only permitted when `objOther` is in Shared mode (`m_pnRefCount != nullptr`). Increments the atomic reference count.

- **Throws:**
  - `SystemException`: If attempted on a uniquely owned pointer.

### `SmartPointer& operator=(const SmartPointer<T>& objOther)` / `template <typename U> SmartPointer& operator=(const SmartPointer<U>& objOther)`
Copy assignment operator. Replaces the managed object and increments the atomic reference count if in Shared mode.

- **Throws:**
  - `SystemException`: If attempted on a uniquely owned pointer.

### `SmartPointer(SmartPointer<T>&& objOther) noexcept` / `template <typename U> SmartPointer(SmartPointer<U>&& objOther) noexcept`
Move constructor. Transfers ownership from `objOther` without altering reference counts.

### `SmartPointer& operator=(SmartPointer<T>&& objOther) noexcept` / `template <typename U> SmartPointer& operator=(SmartPointer<U>&& objOther) noexcept`
Move assignment operator. Releases the current object and transfers ownership from `objOther`.

---

## Member Functions

### `T* Get() const`
Returns the underlying raw pointer without transferring ownership.

### `bool IsNull() const`
Checks if the smart pointer currently manages a null object.

### `int GetRefCount() const`
Returns the current number of shared owners referencing this instance. Returns `0` for unique or null pointers.

### `void Reset(T* pPtr = nullptr)`
Releases the currently owned object and resets the pointer to null or a new object in Unique mode.

### `void Reset(T* pPtr, bool bIsShared)`
Releases the currently owned object and takes ownership of `pPtr` with the specified ownership mode.

### `void Attach(T* pPtr, bool bIsShared = false)`
Attaches a raw pointer to manage, equivalent to `Reset(pPtr, bIsShared)`.

### `T* Detach()`
Detaches the managed object and returns the raw pointer. The `SmartPointer` instance will no longer own the object.

### `template <typename U> SmartPointer<U> DynamicCast() const`
Dynamically casts the managed pointer to type `U*` using `dynamic_cast` and returns a new `SmartPointer<U>` sharing ownership.

---

## Operators

### `T* operator->() const`
Member access operator.

### `T& operator*() const`
Indirection operator.

### `explicit operator bool() const`
Contextual boolean conversion operator (`true` if non-null).

### `template <typename U> bool operator==(const SmartPointer<U>& other) const`
Checks for pointer equality between two smart pointers.

### `template <typename U> bool operator!=(const SmartPointer<U>& other) const`
Checks for pointer inequality between two smart pointers.

---

## Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/String.h"

using namespace DotNetDupe::System;

class DatabaseConnection {
public:
    void Connect() {
        Console::WriteLine("Connected to Database.");
    }
};

int main() {
    // 1. Unique Ownership
    auto pUnique = SmartPointer<DatabaseConnection>::NewUnique();
    pUnique->Connect();

    // 2. Shared Ownership
    auto spShared1 = SmartPointer<DatabaseConnection>::NewShared();
    {
        auto spShared2 = spShared1; // Shared ownership count = 2
        Console::WriteLine("Reference count: {0}", spShared1.GetRefCount());
    } // spShared2 leaves scope, count = 1

    Console::WriteLine("Final reference count: {0}", spShared1.GetRefCount());
    return 0;
}
```
