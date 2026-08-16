# WaitHandle, EventWaitHandle, AutoResetEvent &amp; ManualResetEvent

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/WaitHandle.h"`, `#include "System/Threading/EventWaitHandle.h"`, `#include "System/Threading/AutoResetEvent.h"`, `#include "System/Threading/ManualResetEvent.h"`

Encapsulates operating system-specific synchronization handles that wait for exclusive access to shared resources and cross-thread event signals.

---

## `WaitHandle` (Abstract Base Class)

### Syntax
```cpp
class WaitHandle : public Object;
```

### Pure Virtual Members
- `virtual bool WaitOne() = 0`: Blocks the current thread until the current `WaitHandle` receives a signal.
- `virtual bool WaitOne(int millisecondsTimeout) = 0`: Blocks the current thread until the current `WaitHandle` receives a signal or until the specified timeout elapses.

---

## `EventWaitHandle`

### Syntax
```cpp
class EventWaitHandle : public WaitHandle;
```

### Constructors
- `EventWaitHandle(bool initialState, bool manualReset)`: Initializes an unnamed event handle.
- `EventWaitHandle(const String& sName, bool initialState = false, bool manualReset = false, bool openAlways = true)`: Initializes a named system event handle.
- `EventWaitHandle(bool initialState, bool manualReset, const String& sName, bool openAlways, bool& bCreatedNew)`: Initializes a named event and indicates whether it was created or opened.

### Methods
- `bool Set()`: Sets the state of the event to signaled, allowing one or more waiting threads to proceed.
- `bool Reset()`: Sets the state of the event to non-signaled, causing threads to block.
- `static SmartPointer<EventWaitHandle> OpenExisting(const String& sName)`: Opens a specified named system event.
- `static bool TryOpenExisting(const String& sName, SmartPointer<EventWaitHandle>& pResult)`: Tries to open an existing named event.

---

## `AutoResetEvent`

Notifies a waiting thread that an event has occurred. Resets automatically to non-signaled after releasing a single waiting thread.

### Constructors
- `AutoResetEvent(bool initialState)`: Initializes an unnamed `AutoResetEvent`.
- `AutoResetEvent(const String& sName, bool initialState = false, bool openAlways = true)`: Initializes a named `AutoResetEvent`.
- `AutoResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew)`: Initializes a named `AutoResetEvent`.

### Static Methods
- `static SmartPointer<AutoResetEvent> OpenExisting(const String& sName)`
- `static bool TryOpenExisting(const String& sName, SmartPointer<AutoResetEvent>& pResult)`

---

## `ManualResetEvent`

Notifies one or more waiting threads that an event has occurred. Remains signaled until manually reset by calling `Reset()`.

### Constructors
- `ManualResetEvent(bool initialState)`: Initializes an unnamed `ManualResetEvent`.
- `ManualResetEvent(const String& sName, bool initialState = false, bool openAlways = true)`: Initializes a named `ManualResetEvent`.
- `ManualResetEvent(bool initialState, const String& sName, bool openAlways, bool& bCreatedNew)`: Initializes a named `ManualResetEvent`.

### Static Methods
- `static SmartPointer<ManualResetEvent> OpenExisting(const String& sName)`
- `static bool TryOpenExisting(const String& sName, SmartPointer<ManualResetEvent>& pResult)`

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    AutoResetEvent autoEvent(false);

    ThreadPool::QueueUserWorkItem([&autoEvent]() {
        Console::WriteLine("Worker thread working...");
        Thread::Sleep(100);
        Console::WriteLine("Worker signaling event...");
        autoEvent.Set();
    });

    Console::WriteLine("Main thread waiting for event signal...");
    autoEvent.WaitOne();
    Console::WriteLine("Main thread received signal. Continuing execution.");

    return 0;
}
```
