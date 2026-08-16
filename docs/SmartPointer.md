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

## Factory Methods (Recommended)

### `template <typename... Args> static SmartPointer<T> NewShared(Args&&... args)`
Creates and constructs a shared reference-counted instance of `T` using in-place constructor arguments.

- **Parameters:**
  - `args`: Arguments forwarded directly to `T`'s constructor.
- **Returns:**
  - `SmartPointer<T>`: A shared pointer referencing the newly allocated instance.

```cpp
auto spFile = SmartPointer<FileStream>::NewShared("data.bin", FileMode::Open);
```

### `template <typename... Args> static SmartPointer<T> NewUnique(Args&&... args)`
Creates and constructs a uniquely owned instance of `T`.

- **Parameters:**
  - `args`: Arguments forwarded directly to `T`'s constructor.
- **Returns:**
  - `SmartPointer<T>`: A unique pointer referencing the newly allocated instance.

```cpp
auto spBuffer = SmartPointer<MemoryStream>::NewUnique();
```

---

## Constructors

### `SmartPointer()`
Initializes a new `SmartPointer` instance. Automatically allocates `new T()` for complete, non-abstract types with unique ownership. For abstract classes, initializes to `nullptr`.

### `explicit SmartPointer(bool bShared)`
Initializes a new `SmartPointer` instance with shared ownership enabled.

### `explicit SmartPointer(T* pRaw, bool bShared = false)`
Takes ownership of an existing raw pointer.

- **Parameters:**
  - `pRaw` (`T*`): Raw pointer to manage.
  - `bShared` (`bool`): If `true`, enables atomic reference counting.

### `SmartPointer(const SmartPointer<T>& other)`
Copy constructor. Only permitted when `other` is configured as a shared pointer (`IsShared() == true`). Increments the atomic reference count.

- **Throws:**
  - `SystemException`: If attempted on a uniquely owned pointer.

### `SmartPointer(SmartPointer<T>&& other) noexcept`
Move constructor. Transfers ownership from `other` without altering reference counts.

---

## Member Functions

### `T* Get() const`
Returns the underlying raw pointer without transferring ownership.

- **Returns:**
  - `T*`: Raw pointer to the managed object, or `nullptr`.

### `bool IsNull() const`
Checks if the smart pointer currently manages a null object.

- **Returns:**
  - `bool`: `true` if `Get() == nullptr`; otherwise, `false`.

```cpp
if (spObj.IsNull()) {
    // Pointer is empty
}
```

### `bool IsShared() const`
Indicates whether this smart pointer instance participates in shared reference counting.

### `long UseCount() const`
Returns the current number of shared owners referencing this instance.

### `void Reset(T* pNew = nullptr, bool bShared = false)`
Releases the currently owned object (deleting it if reference count reaches 0) and takes ownership of `pNew`.

### `T* operator->() const`
Member access operator.

- **Throws:**
  - `NullReferenceException`: If the pointer is null.

### `T& operator*() const`
Indirection operator.

- **Throws:**
  - `NullReferenceException`: If the pointer is null.

### `explicit operator bool() const`
Contextual boolean conversion operator (`true` if non-null).

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
        Console::WriteLine("Reference count: {0}", spShared1.UseCount());
    } // spShared2 leaves scope, count = 1

    Console::WriteLine("Final reference count: {0}", spShared1.UseCount());
    return 0;
}
```
