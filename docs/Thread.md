# Thread

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/Thread.h"`

Creates and controls a thread, sets its priority, and gets its execution status.

---

## Syntax

```cpp
typedef Action<> ThreadStart;
typedef Action<Object*> ParameterizedThreadStart;

class Thread : public Object;
```

---

## Constructors

### `Thread(ThreadStart start)`
Initializes a new instance of the `Thread` class with an unparameterized entry delegate.

### `Thread(ParameterizedThreadStart start)`
Initializes a new instance of the `Thread` class with a delegate that takes an `Object*` argument.

---

## Member Functions

### `void Start()`
Causes the operating system to change the state of the current instance to Running.

### `void Start(Object* parameter)`
Causes the operating system to change the state of the current instance to Running, supplying an object containing data to be used by the thread method.

### `void Join()`
Blocks the calling thread until the thread represented by this instance terminates.

### `bool Join(int millisecondsTimeout)`
Blocks the calling thread until the thread terminates or the specified time elapses.
- **Returns:**
  - `bool`: `true` if the thread has terminated; `false` if the thread has not terminated after `millisecondsTimeout` milliseconds.
- **Throws:**
  - `TimeoutException`: If a wait failure occurs before the thread terminates.

### `bool IsAlive() const`
Gets a value indicating the execution status of the current thread.

### `String GetName() const` / `void SetName(const String& name)`
Gets or sets the human-readable display name of the thread.

---

## Static Methods

### `static void Sleep(int millisecondsTimeout)`
Suspends the current thread for the specified number of milliseconds.

### `static Thread* GetCurrentThread()`
Gets the currently running thread.

### `static int GetCurrentThreadId()`
Gets an integer identifier for the current managed/native thread.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

void WorkerTask() {
    Console::WriteLine("Worker thread [ID: {0}] started...", Thread::GetCurrentThreadId());
    Thread::Sleep(100);
    Console::WriteLine("Worker thread work finished.");
}

int main() {
    Thread thWorker(WorkerTask);
    thWorker.SetName("BackgroundWorker");
    thWorker.Start();

    Console::WriteLine("Main thread waiting on {0}...", thWorker.GetName());
    thWorker.Join();
    Console::WriteLine("Worker thread joined successfully.");

    return 0;
}
```
