# Mutex

A synchronization primitive that can also be used for inter-process synchronization.

## Methods

### `Mutex()`
Initializes a new instance of the `Mutex` class with default settings.

### `Mutex(bool bInitiallyOwned)`
Initializes a new instance of the `Mutex` class with an option to own it initially.

### `Mutex(bool bInitiallyOwned, const String& sName)`
Initializes a new instance of the `Mutex` class with a specified name.

### `Mutex(bool bInitiallyOwned, const String& sName, bool& bCreatedNew)`
Initializes a new instance of the `Mutex` class with a specified name and returns whether a new instance was created.

### `Mutex(const String& sName, bool openExisting = true)`
Opens an existing named `Mutex` using system `OpenMutex`. If `openExisting` is set to `false`, throws `WaitHandleCannotBeOpenedException`.

### `static Mutex* OpenExisting(const String& sName)`
Opens an existing named `Mutex`. Throws `WaitHandleCannotBeOpenedException` if the named handle does not exist.

### `static bool TryOpenExisting(const String& sName, Mutex*& pResult)`
Attempts to open an existing named `Mutex`, returning `true` if successful.

### `bool WaitOne()`
Blocks the current thread until the current `Mutex` is acquired.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `Mutex` is acquired or the timeout expires.

**Exceptions:**
- `TimeoutException`: Thrown if the timeout expires.

### `int Release(int releaseCount = 1)`
Releases the `Mutex` once. The `releaseCount` parameter is ignored for `Mutex` but provided for compatibility with the `LockWaitHandle` base class.

## Code Example

```cpp
#include "System/Threading/Mutex.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/TimeoutException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("Mutex demo started.");

    // Create a shared Mutex using SmartPointer
    auto pMutex = SmartPointer<Mutex>::NewShared();

    // Start a thread that holds the mutex for a short duration
    SmartPointer<Thread> pThread1 = SmartPointer<Thread>::New([pMutex]() {
        pMutex->WaitOne();
        Console::WriteLine("Thread 1 acquired Mutex, holding for 150ms...");
        Thread::Sleep(150);
        Console::WriteLine("Thread 1 releasing Mutex...");
        pMutex->Release();
    });

    pThread1->Start();

    // Sleep a bit to let Thread 1 acquire it first
    Thread::Sleep(50);

    // Try to acquire the mutex in the main thread with a short timeout (which should fail)
    try {
        Console::WriteLine("Main thread trying to acquire Mutex (50ms timeout)...");
        pMutex->WaitOne(50);
        Console::WriteLine("Main thread acquired Mutex (unexpected).");
        pMutex->Release();
    } catch (const TimeoutException& ex) {
        Console::Write("Main thread expected timeout exception: ");
        Console::WriteLine(ex.What());
    }

    // Wait for Thread 1 to finish
    pThread1->Join();

    // Now try to acquire it in the main thread
    pMutex->WaitOne();
    Console::WriteLine("Main thread successfully acquired Mutex after Thread 1 released it.");
    pMutex->Release();

    Console::WriteLine("Mutex demo completed.");
    return 0;
}
```


