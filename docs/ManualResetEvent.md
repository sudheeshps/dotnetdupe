# ManualResetEvent

Notifies one or more waiting threads that an event has occurred. This class cannot be inherited.

## Methods

### `ManualResetEvent(bool initialState)`
Initializes a new instance of the `ManualResetEvent` class with a Boolean value signaling whether to set the initial state to signaled.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current event receives a signal.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out.

## Remarks
When a `ManualResetEvent` is signaled, it remains signaled until it is manually reset by calling the `Reset` method.

## Code Example

```cpp
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("ManualResetEvent demo started.");

    // Create a ManualResetEvent in nonsignaled state
    auto pMre = SmartPointer<ManualResetEvent>::NewShared(false);

    // Lambda for worker threads
    auto runWorker = [pMre](Object* pState) {
        int iId = (int)(intptr_t)pState;
        Console::Write("Worker ");
        Console::Write(iId);
        Console::WriteLine(" waiting for signal...");
        
        pMre->WaitOne();
        
        Console::Write("Worker ");
        Console::Write(iId);
        Console::WriteLine(" received signal and is proceeding.");
    };

    SmartPointer<Thread> pT1 = SmartPointer<Thread>::New(runWorker);
    SmartPointer<Thread> pT2 = SmartPointer<Thread>::New(runWorker);

    pT1->Start(reinterpret_cast<Object*>(1));
    pT2->Start(reinterpret_cast<Object*>(2));

    // Let the worker threads start waiting
    Thread::Sleep(50);

    Console::WriteLine("Main thread signaling the event (releases both threads)...");
    pMre->Set();

    pT1->Join();
    pT2->Join();

    // Since it's a manual reset event, it remains signaled
    Console::WriteLine("Verifying that it remains signaled...");
    pMre->WaitOne();
    Console::WriteLine("WaitOne completed immediately without blocking.");

    // Now reset it
    Console::WriteLine("Resetting event...");
    pMre->Reset();

    Console::WriteLine("ManualResetEvent demo completed.");
    return 0;
}
```


