# Semaphore

Limits the number of threads that can access a resource or pool of resources concurrently.

## Methods

### `Semaphore(int initialCount, int maximumCount)`
Initializes a new instance of the `Semaphore` class, specifying the initial number of entries and the maximum number of concurrent entries.

### `Semaphore(int initialCount, int maximumCount, const String& sName)`
Initializes a new instance of the `Semaphore` class with a specified name.

### `Semaphore(int initialCount, int maximumCount, const String& sName, bool& bCreatedNew)`
Initializes a new instance of the `Semaphore` class with a specified name, returning whether a new handle was created.

### `static Semaphore* OpenExisting(const String& sName)`
Opens an existing named `Semaphore`. Throws `WaitHandleCannotBeOpenedException` if the named handle does not exist.

### `static bool TryOpenExisting(const String& sName, Semaphore*& pResult)`
Attempts to open an existing named `Semaphore`, returning `true` if successful.

### `bool WaitOne()`
Blocks the current thread until it can enter the `Semaphore`.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until it can enter the `Semaphore` or the timeout expires.

**Exceptions:**
- `TimeoutException`: Thrown if the timeout expires.

### `int Release(int releaseCount = 1)`
Exits the semaphore a specified number of times and returns the previous count.

**Exceptions:**
- `SemaphoreFullException`: Thrown if the count would exceed the maximum count.

## Code Example

```cpp
#include "System/Threading/Semaphore.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Threading/SemaphoreFullException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("Semaphore demo started.");

    // Create a Semaphore with a capacity of 2
    auto pSemaphore = SmartPointer<Semaphore>::NewShared(2, 2);

    // Thread 1
    SmartPointer<Thread> pT1 = SmartPointer<Thread>::New([pSemaphore]() {
        Console::WriteLine("Thread 1 waiting to enter semaphore...");
        pSemaphore->WaitOne();
        Console::WriteLine("Thread 1 entered semaphore.");
        Thread::Sleep(100);
        Console::WriteLine("Thread 1 releasing semaphore.");
        pSemaphore->Release();
    });

    // Thread 2
    SmartPointer<Thread> pT2 = SmartPointer<Thread>::New([pSemaphore]() {
        Console::WriteLine("Thread 2 waiting to enter semaphore...");
        pSemaphore->WaitOne();
        Console::WriteLine("Thread 2 entered semaphore.");
        Thread::Sleep(100);
        Console::WriteLine("Thread 2 releasing semaphore.");
        pSemaphore->Release();
    });

    // Thread 3 (Will wait until T1 or T2 releases)
    SmartPointer<Thread> pT3 = SmartPointer<Thread>::New([pSemaphore]() {
        Console::WriteLine("Thread 3 waiting to enter semaphore...");
        pSemaphore->WaitOne();
        Console::WriteLine("Thread 3 entered semaphore.");
        Thread::Sleep(100);
        Console::WriteLine("Thread 3 releasing semaphore.");
        pSemaphore->Release();
    });

    pT1->Start();
    pT2->Start();
    pT3->Start();

    pT1->Join();
    pT2->Join();
    pT3->Join();

    // Demonstrate SemaphoreFullException
    try {
        pSemaphore->Release(); // Releases beyond max count of 2
    } catch (const SemaphoreFullException& ex) {
        Console::Write("Expected SemaphoreFullException: ");
        Console::WriteLine(ex.What());
    }

    Console::WriteLine("Semaphore demo completed.");
    return 0;
}
```


