# ThreadPool

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/ThreadPool.h"`

Provides a pool of threads that can be used to execute tasks, post work items, wait on behalf of other threads, and process concurrent operations efficiently without thread-spawning overhead.

---

## Syntax

```cpp
typedef Action<Object*> WaitCallback;

class ThreadPool : public Object;
```

---

## Static Methods

### `static bool QueueUserWorkItem(WaitCallback callback)`
Queues a method for execution. The method executes when a thread pool worker becomes available.
- **Parameters:**
  - `callback` (`WaitCallback`): An `Action<Object*>` that represents the delegate to execute.
- **Returns:**
  - `bool`: `true` if the method is successfully queued; otherwise, `false`.

```cpp
ThreadPool::QueueUserWorkItem([](Object* state) {
    Console::WriteLine("Executing async background task on thread pool");
});
```

### `static bool QueueUserWorkItem(WaitCallback callback, Object* pState)`
Queues a method for execution, and specifies an object containing context data to be used by the method.

### `static bool SetMinThreads(int iMinThreads)`
Sets the minimum number of worker threads the thread pool creates on demand.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    ThreadPool::SetMinThreads(4);

    for (int i = 1; i <= 3; ++i) {
        ThreadPool::QueueUserWorkItem([](Object* state) {
            Console::WriteLine("Running worker task on Thread #{0}", Thread::GetCurrentThreadId());
            Thread::Sleep(50);
        });
    }

    Thread::Sleep(200); // Allow thread pool workers to finish
    Console::WriteLine("All pool tasks completed.");

    return 0;
}
```
