# Semaphore &amp; SemaphoreSlim

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/Semaphore.h"`, `#include "System/Threading/SemaphoreSlim.h"`

Limits the number of threads that can access a resource or pool of resources concurrently. `Semaphore` wraps OS-level synchronization handles (supporting cross-process sharing), while `SemaphoreSlim` is a lightweight, low-overhead intra-process alternative.

---

## `Semaphore`

### Syntax
```cpp
class Semaphore : public LockWaitHandle;
```

### Constructors
- `Semaphore(int initialCount, int maximumCount)`: Initializes an unnamed semaphore with initial and maximum concurrent counts.
- `Semaphore(const String& sName, int initialCount = 0, int maximumCount = 1, bool openAlways = true)`: Initializes a named system semaphore.
- `Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways = true)`: Initializes a named semaphore specifying initial/max counts and open flag.
- `Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways, bool& bCreatedNew)`: Initializes a named semaphore and indicates whether it was created or opened.

### Member Functions
- `bool WaitOne() override`: Blocks until the semaphore counter is decremented (slot acquired).
  - **Throws:**
    - `UnauthorizedAccessException`: If access is denied by OS permissions.
    - `WaitHandleCannotBeOpenedException`: If the handle is invalid.
- `bool WaitOne(int millisecondsTimeout) override`: Blocks until acquired or timeout elapses.
  - **Throws:**
    - `TimeoutException`: If a wait failure occurs.
    - `UnauthorizedAccessException`: If access is denied.
- `int Release(int releaseCount = 1) override`: Exits the semaphore and returns the previous count.
  - **Throws:**
    - `SemaphoreFullException`: If the release count exceeds the maximum semaphore capacity.

### Static Methods
- `static SmartPointer<Semaphore> OpenExisting(const String& sName)`
- `static bool TryOpenExisting(const String& sName, SmartPointer<Semaphore>& pResult)`

---

## `SemaphoreSlim`

### Syntax
```cpp
class SemaphoreSlim : public LockWaitHandle;
```

### Constructors
- `SemaphoreSlim(int initialCount)`: Initializes a `SemaphoreSlim` with initial and maximum count set to `initialCount`.
- `SemaphoreSlim(int initialCount, int maximumCount)`: Initializes a `SemaphoreSlim` specifying both initial and maximum slots.

### Member Functions
- `bool WaitOne() override`: Blocks until a slot is available.
- `bool WaitOne(int millisecondsTimeout) override`: Blocks until slot is available or timeout occurs.
- `int Release(int releaseCount = 1) override`: Releases slots and returns previous count.
- `int GetCurrentCount() const`: Returns the number of remaining free slots available.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    // Allow at most 2 concurrent workers
    SemaphoreSlim semThrottler(2, 2);

    for (int iIdx = 1; iIdx <= 5; ++iIdx) {
        ThreadPool::QueueUserWorkItem([&semThrottler, iIdx](Object* pState) {
            semThrottler.WaitOne();
            Console::WriteLine("Worker {0} entered critical section (Slots left: {1})", iIdx, semThrottler.GetCurrentCount());
            Thread::Sleep(50);
            semThrottler.Release();
            Console::WriteLine("Worker {0} released slot.", iIdx);
        });
    }

    Thread::Sleep(300);
    return 0;
}
```
