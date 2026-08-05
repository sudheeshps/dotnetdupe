# EventWaitHandle

Represents a thread synchronization event.

## Methods

### `EventWaitHandle(bool initialState, bool manualReset)`
Initializes a new instance of the `EventWaitHandle` class, specifying whether the wait handle is initially signaled, and whether it resets automatically or manually.

### `EventWaitHandle(bool initialState, bool manualReset, const String& sName)`
Initializes a new instance of the `EventWaitHandle` class with a system-wide or process-registered name.

### `EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool& bCreatedNew)`
Initializes a new instance of the `EventWaitHandle` class with a specified name, setting `bCreatedNew` to `true` if a new kernel object was created.

### `static EventWaitHandle* OpenExisting(const String& sName)`
Opens an existing named `EventWaitHandle`. Throws `WaitHandleCannotBeOpenedException` if the named handle does not exist.

### `static bool TryOpenExisting(const String& sName, EventWaitHandle*& pResult)`
Attempts to open an existing named `EventWaitHandle`, returning `true` if successful.

### `bool Set()`
Sets the state of the event to signaled, allowing one or more waiting threads to proceed.

### `bool Reset()`
Sets the state of the event to nonsignaled, causing threads to block.

### `bool WaitOne()`
Blocks the current thread until the current `WaitHandle` receives a signal.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `WaitHandle` receives a signal, using a 32-bit signed integer to specify the time interval.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out.

## Code Example

```cpp
#include "System/Threading/EventWaitHandle.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("EventWaitHandle demo started.");

    // Create an EventWaitHandle as a manual reset event (manualReset = true)
    auto pEwh = SmartPointer<EventWaitHandle>::NewShared(false, true);

    // Spawn a worker thread that waits on the event
    SmartPointer<Thread> pWorker = SmartPointer<Thread>::New([pEwh]() {
        Console::WriteLine("Worker thread waiting for signal...");
        pEwh->WaitOne();
        Console::WriteLine("Worker thread received signal and is executing.");
    });

    pWorker->Start();

    // Sleep a bit to let worker thread start waiting
    Thread::Sleep(100);

    Console::WriteLine("Main thread signaling the EventWaitHandle...");
    pEwh->Set();

    pWorker->Join();

    Console::WriteLine("EventWaitHandle demo completed.");
    return 0;
}
```


