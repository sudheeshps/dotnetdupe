# CriticalSection &amp; Lock&lt;T&gt;

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/CriticalSection.h"`, `#include "System/Threading/Lock.h"`

`CriticalSection` provides a lightweight, recursive synchronization primitive for threads within the same process. `Lock<T>` provides exception-safe RAII scoping for synchronization objects.

---

## `CriticalSection`

### Syntax
```cpp
class CriticalSection : public Object;
```

### Constructors
- `CriticalSection()`: Initializes a new critical section object.

### Member Functions
- `void Enter()`: Waits for ownership of the specified critical section object.
- `void Leave()`: Releases ownership of the critical section object.
- `bool TryEnter()`: Attempts to enter a critical section without blocking. Returns `true` if the thread entered successfully; otherwise, `false`.

---

## `Lock<T>` (RAII Lock Wrapper)

### Syntax
```cpp
template <typename T>
class Lock;

typedef Lock<CriticalSection> CriticalSectionLock;
typedef Lock<Mutex> MutexLock;
typedef Lock<Semaphore> SemaphoreLock;
typedef Lock<SemaphoreSlim> SemaphoreSlimLock;
```

### Constructors
- `Lock(T& syncObject, int millisecondsTimeout = -1, int releaseCount = -1)`: Acquires `syncObject` upon construction.
- `~Lock()`: Releases `syncObject` automatically when leaving scope (even if an exception is thrown).

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Threading/ThreadPool.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

CriticalSection g_cs;
int g_counter = 0;

void IncrementSharedData() {
    // RAII Lock acquisition
    CriticalSectionLock lock(g_cs);
    g_counter++;
    Console::WriteLine("Counter updated safely to: {0}", g_counter);
}

int main() {
    for (int i = 0; i < 5; ++i) {
        ThreadPool::QueueUserWorkItem([](Object* state) {
            IncrementSharedData();
        });
    }

    Thread::Sleep(150);
    return 0;
}
```
