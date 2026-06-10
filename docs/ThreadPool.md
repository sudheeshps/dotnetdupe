### class `ThreadPool`

Provides a pool of threads that can be used to execute tasks, post work items, process asynchronous I/O, wait on behalf of other threads, and process timers.

#### Methods

##### `static bool QueueUserWorkItem(WaitCallback callback)`

Queues a method for execution. The method executes when a thread pool thread becomes available.

**Usage:**
```cpp
ThreadPool::QueueUserWorkItem(WaitCallback([](Object* state) {
    Console::WriteLine("Running on thread pool...");
}));
```

##### `static bool QueueUserWorkItem(WaitCallback callback, Object* pState)`

Queues a method for execution, and specifies an object containing data to be used by the method. The method executes when a thread pool thread becomes available.

**Usage:**
```cpp
Object* pData = reinterpret_cast<Object*>(42);
ThreadPool::QueueUserWorkItem(WaitCallback([](Object* state) {
    int iVal = (int)(intptr_t)state;
    Console::Write("Received value: ");
    Console::WriteLine(iVal);
}), pData);
```

##### `typedef Action<Object*> WaitCallback`

Represents a callback method to be executed by a thread pool thread.

## Code Example

```cpp
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("ThreadPool demo started.");

    // Queue a work item without state
    ThreadPool::QueueUserWorkItem(WaitCallback([](Object* pState) {
        Console::WriteLine("Work item 1 running on the ThreadPool.");
    }));

    // Queue a work item with state
    Object* pStateObj = reinterpret_cast<Object*>(42);
    ThreadPool::QueueUserWorkItem(WaitCallback([](Object* pState) {
        intptr_t iVal = reinterpret_cast<intptr_t>(pState);
        Console::Write("Work item 2 running with state: ");
        Console::WriteLine((int)iVal);
    }), pStateObj);

    // Give the thread pool time to process the queued items
    Thread::Sleep(200);

    Console::WriteLine("ThreadPool demo completed.");
    return 0;
}
```

