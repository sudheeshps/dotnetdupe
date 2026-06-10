# CriticalSection

A high-performance user-mode synchronization primitive using Win32 `CRITICAL_SECTION`.

## Methods

### `CriticalSection()`
Initializes a new instance of the `CriticalSection` class with a spin count for optimized performance.

### `void Enter()`
Enters the critical section. Blocks if another thread has entered.

### `void Leave()`
Leaves the critical section.

### `bool TryEnter()`
Attempts to enter the critical section without blocking.

**Returns:**
- `true` if the thread successfully entered; otherwise, `false`.

## Code Example

```cpp
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("CriticalSection demo started.");

    // Create a shared CriticalSection using SmartPointer
    auto pCs = SmartPointer<CriticalSection>::NewShared();
    
    // Shared resource
    int iSharedData = 0;

    // Spawn worker threads to update shared resource
    auto runWorker = [pCs, &iSharedData](Object* pState) {
        int iId = (int)(intptr_t)pState;
        
        for (int iI = 0; iI < 3; ++iI) {
            // Using CriticalSectionLock for RAII-based safety
            {
                CriticalSectionLock lock(*pCs);
                iSharedData++;
                Console::Write("Thread ");
                Console::Write(iId);
                Console::Write(" updated data to: ");
                Console::WriteLine(iSharedData);
            }
            Thread::Sleep(30);
        }
    };

    SmartPointer<Thread> pT1 = SmartPointer<Thread>::New(runWorker);
    SmartPointer<Thread> pT2 = SmartPointer<Thread>::New(runWorker);

    pT1->Start(reinterpret_cast<Object*>(1));
    pT2->Start(reinterpret_cast<Object*>(2));

    pT1->Join();
    pT2->Join();

    // Demonstration of direct TryEnter / Enter / Leave
    if (pCs->TryEnter()) {
        Console::WriteLine("Main thread successfully entered CriticalSection via TryEnter.");
        pCs->Leave();
    } else {
        Console::WriteLine("Main thread failed to enter CriticalSection (unexpected).");
    }

    Console::WriteLine("CriticalSection demo completed.");
    return 0;
}
```


