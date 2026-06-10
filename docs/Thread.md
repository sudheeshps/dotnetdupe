### class `Thread`

Creates and controls a thread, sets its priority, and gets its status.

#### Methods

##### `Thread(ThreadStart start)`

Initializes a new instance of the `Thread` class.

**Usage:**
```cpp
Thread thread([]() { /* ... */ });
```

##### `Thread(ParameterizedThreadStart start)`

Initializes a new instance of the `Thread` class, specifying a delegate that allows an object to be passed to the thread when the thread is started.

**Usage:**
```cpp
Thread thread([](Object* obj) { /* ... */ });
```

##### `void Start()`

Causes the operating system to change the state of the current instance to `ThreadState::Running`.

**Usage:**
```cpp
thread.Start();
```

##### `void Start(Object* parameter)`

Causes the operating system to change the state of the current instance to `ThreadState::Running`, and optionally supplies an object containing data to be used by the method the thread executes.

**Usage:**
```cpp
thread.Start(myObject);
```

##### `void Join()`

Blocks the calling thread until the thread represented by this instance terminates.

**Usage:**
```cpp
thread.Join();
```

##### `bool Join(int millisecondsTimeout)`

Blocks the calling thread until the thread represented by this instance terminates or the specified time elapses.

**Returns:**
- `true` if the thread has terminated; otherwise, `false`. (Note: In this implementation, it throws `TimeoutException` if it expires).

**Exceptions:**
- `TimeoutException`: Thrown if the thread does not terminate within the specified time.

**Usage:**
```cpp
try {
    thread.Join(1000);
} catch (const TimeoutException& ex) {
    // Handle timeout
}
```

##### `static void Sleep(int millisecondsTimeout)`

Suspends the current thread for the specified number of milliseconds.

**Usage:**
```cpp
Thread::Sleep(1000);
```

##### `bool IsAlive() const`

Gets a value indicating the execution status of the current thread.

**Usage:**
```cpp
if (thread.IsAlive()) { /* ... */ }
```

##### `static Thread* GetCurrentThread()`

Gets the currently running thread.

**Usage:**
```cpp
Thread* current = Thread::GetCurrentThread();
```

## Code Example

```cpp
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("Thread demo started.");

    // Create a background thread using SmartPointer
    SmartPointer<Thread> pWorker = SmartPointer<Thread>::New([]() {
        Thread* pCurrent = Thread::GetCurrentThread();
        pCurrent->SetName("Worker-Thread");
        
        Console::Write("Thread '");
        Console::Write(pCurrent->GetName());
        Console::WriteLine("' is working...");
        
        Thread::Sleep(100);
        Console::WriteLine("Worker thread task complete.");
    });

    Console::WriteLine("Starting worker thread...");
    pWorker->Start();

    Console::WriteLine("Waiting for worker thread to finish...");
    pWorker->Join();

    Console::WriteLine("Thread demo completed.");
    return 0;
}
```


