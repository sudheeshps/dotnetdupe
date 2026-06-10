# WaitHandle

The `WaitHandle` class is an abstract base class for synchronization objects that wait for exclusive access to shared resources.

## Methods

### `virtual bool WaitOne()`
Blocks the current thread until the current `WaitHandle` receives a signal.

**Returns:**
- `true` if the current instance receives a signal.

### `virtual bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `WaitHandle` receives a signal, using a 32-bit signed integer to specify the time interval.

**Parameters:**
- `millisecondsTimeout`: The number of milliseconds to wait, or -1 to wait indefinitely.

**Returns:**
- `true` if the current instance receives a signal; otherwise, `false`.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out (for derived classes that implement timeout exceptions).

---

## Code Example

The following example shows how to use a `WaitHandle` (using its concrete subclass `ManualResetEvent`) to synchronize a background worker thread.

```cpp
#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

void Worker(SmartPointer<WaitHandle> waitHandle) {
    Console::WriteLine("Worker: Waiting for the signal...");
    waitHandle->WaitOne();
    Console::WriteLine("Worker: Signal received, proceeding with work!");
}

int main() {
    // Create a concrete WaitHandle subclass in shared mode
    SmartPointer<WaitHandle> mre = SmartPointer<ManualResetEvent>::NewShared(false);

    // Start background thread passing the WaitHandle
    Thread t([mre]() {
        Worker(mre);
    });
    t.Start();

    // Sleep to simulate work, then signal
    Thread::Sleep(500);
    Console::WriteLine("Main: Signaling the WaitHandle...");
    
    // Cast and set (signal)
    static_cast<ManualResetEvent*>(mre.Get())->Set();

    t.Join();
    return 0;
}
```


