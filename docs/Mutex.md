# Mutex

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/Mutex.h"`

A synchronization primitive that grants exclusive access to a shared resource to only one thread at a time. Supports named inter-process synchronization across multiple running processes.

---

## Syntax

```cpp
class Mutex : public LockWaitHandle;
```

---

## Constructors

### `Mutex()`
Initializes a new, unnamed instance of the `Mutex` class with default properties.

### `Mutex(bool bInitiallyOwned)`
Initializes a new, unnamed instance with a value indicating whether the calling thread should have initial ownership of the mutex.

### `Mutex(const String& sName, bool bInitiallyOwned = false, bool openAlways = true)`
Initializes a new named instance of the `Mutex` class.

### `Mutex(bool bInitiallyOwned, const String& sName, bool openAlways = true)`
Initializes a new named instance specifying initial ownership and whether to open an existing named mutex.

### `Mutex(bool bInitiallyOwned, const String& sName, bool openAlways, bool& bCreatedNew)`
Initializes a named `Mutex` and returns whether a new mutex was created or an existing one was opened.

---

## Member Functions

### `bool WaitOne() override`
Blocks the calling thread until the thread acquires ownership of the `Mutex`.
- **Throws:**
  - `UnauthorizedAccessException`: If access is denied by OS permissions.
  - `WaitHandleCannotBeOpenedException`: If the handle is invalid.

### `bool WaitOne(int millisecondsTimeout) override`
Blocks the calling thread until ownership is acquired or the timeout elapses.
- **Returns:**
  - `bool`: `true` if ownership was acquired; otherwise, `false`.
- **Throws:**
  - `TimeoutException`: If a wait failure occurs.
  - `UnauthorizedAccessException`: If access is denied by OS permissions.

### `int Release(int releaseCount = 1) override`
Releases the `Mutex`.

---

## Static Methods

### `static SmartPointer<Mutex> OpenExisting(const String& sName)`
Opens the specified existing named mutex.
- **Throws:**
  - `WaitHandleCannotBeOpenedException`: If the named mutex does not exist.

### `static bool TryOpenExisting(const String& sName, SmartPointer<Mutex>& pResult)`
Opens the specified existing named mutex without throwing an exception if it does not exist.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    bool bCreatedNew = false;
    Mutex mtxSingleInstance(true, "Global_MyApplication_SingleInstanceLock", true, bCreatedNew);

    if (!bCreatedNew) {
        Console::WriteLine("Another instance of this application is already running!");
        return 1;
    }

    Console::WriteLine("Acquired single-instance application mutex. Application running.");
    Thread::Sleep(100);

    mtxSingleInstance.Release();
    return 0;
}
```
