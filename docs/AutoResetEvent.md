# AutoResetEvent

Notifies a waiting thread that an event has occurred. This class cannot be inherited.

## Methods

### `AutoResetEvent(bool initialState)`
Initializes a new instance of the `AutoResetEvent` class with a Boolean value signaling whether to set the initial state to signaled.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current event receives a signal.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out.

## Remarks
When an `AutoResetEvent` is signaled, it automatically resets to nonsignaled after a single waiting thread has been released.

## Code Example

```cpp
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/TimeoutException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("AutoResetEvent demo started.");

    // Create an AutoResetEvent in nonsignaled state
    auto pAre = SmartPointer<AutoResetEvent>::NewShared(false);

    // Spawn a worker thread that waits on the event
    SmartPointer<Thread> pWorker = SmartPointer<Thread>::New([pAre]() {
        Console::WriteLine("Worker thread waiting for signal...");
        pAre->WaitOne();
        Console::WriteLine("Worker thread received signal and is executing.");
        
        // WaitOne on an AutoResetEvent automatically resets it to nonsignaled, 
        // so a subsequent wait will block or timeout.
        try {
            Console::WriteLine("Worker thread waiting again (should timeout)...");
            pAre->WaitOne(50);
            Console::WriteLine("Worker thread received second signal (unexpected).");
        } catch (const TimeoutException& ex) {
            Console::Write("Worker thread expected timeout: ");
            Console::WriteLine(ex.What());
        }
    });

    pWorker->Start();

    // Let the worker thread start waiting
    Thread::Sleep(50);

    Console::WriteLine("Main thread signaling the event...");
    pAre->Set();

    pWorker->Join();

    Console::WriteLine("AutoResetEvent demo completed.");
    return 0;
}
```


