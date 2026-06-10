# SemaphoreSlim

Represents a lightweight alternative to `Semaphore` that limits the number of threads that can access a resource or pool of resources concurrently.

## Methods

### `SemaphoreSlim(int initialCount)`
Initializes a new instance of the `SemaphoreSlim` class, specifying the initial number of requests that can be granted concurrently.

### `SemaphoreSlim(int initialCount, int maximumCount)`
Initializes a new instance of the `SemaphoreSlim` class, specifying the initial and maximum number of requests that can be granted concurrently.

### `bool WaitOne()`
Blocks the current thread until it can enter the `SemaphoreSlim`.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until it can enter the `SemaphoreSlim` or the timeout expires.

**Exceptions:**
- `TimeoutException`: Thrown if the timeout expires.

### `int Release(int releaseCount = 1)`
Releases the `SemaphoreSlim` object a specified number of times.

### `int GetCurrentCount()`
Gets the number of remaining threads that can enter the `SemaphoreSlim` object.

## Code Example

```cpp
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/TimeoutException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("SemaphoreSlim demo started.");

    // Create a SemaphoreSlim with initial count 1, max count 2
    auto pSemaphoreSlim = SmartPointer<SemaphoreSlim>::NewShared(1, 2);

    Console::Write("Initial count: ");
    Console::WriteLine(pSemaphoreSlim->GetCurrentCount());

    // Acquire the single slot
    pSemaphoreSlim->WaitOne();
    Console::Write("Count after WaitOne: ");
    Console::WriteLine(pSemaphoreSlim->GetCurrentCount());

    // Spawn a thread to try acquiring with a timeout (should time out because count is 0)
    SmartPointer<Thread> pThread = SmartPointer<Thread>::New([pSemaphoreSlim]() {
        try {
            Console::WriteLine("Thread attempting to enter SemaphoreSlim (50ms timeout)...");
            pSemaphoreSlim->WaitOne(50);
            Console::WriteLine("Thread entered SemaphoreSlim (unexpected).");
            pSemaphoreSlim->Release();
        } catch (const TimeoutException& ex) {
            Console::Write("Thread expected timeout: ");
            Console::WriteLine(ex.What());
        }
    });

    pThread->Start();
    pThread->Join();

    // Release and increment beyond initial but within max
    pSemaphoreSlim->Release(2);
    Console::Write("Count after releasing 2: ");
    Console::WriteLine(pSemaphoreSlim->GetCurrentCount());

    // Clean up by entering again
    pSemaphoreSlim->WaitOne();
    pSemaphoreSlim->WaitOne();
    Console::Write("Count after acquiring both: ");
    Console::WriteLine(pSemaphoreSlim->GetCurrentCount());

    pSemaphoreSlim->Release(2);

    Console::WriteLine("SemaphoreSlim demo completed.");
    return 0;
}
```


