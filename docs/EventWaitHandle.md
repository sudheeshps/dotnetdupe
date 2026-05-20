# EventWaitHandle

Represents a thread synchronization event.

## Methods

### `EventWaitHandle(bool initialState, bool manualReset)`
Initializes a new instance of the `EventWaitHandle` class, specifying whether the wait handle is initially signaled, and whether it resets automatically or manually.

### `bool Set()`
Sets the state of the event to signaled, allowing one or more waiting threads to proceed.

### `bool Reset()`
Sets the state of the event to nonsignaled, causing threads to block.

### `bool WaitOne()`
Blocks the current thread until the current `WaitHandle` receives a signal.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `WaitHandle` receives a signal, using a 32-bit signed integer to specify the time interval.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

EventWaitHandle ewh(false, true); // Manual reset

Thread t([&ewh]() {
    ewh.WaitOne();
    // Do work
});
t.Start();

ewh.Set(); // Signal thread to start
```
