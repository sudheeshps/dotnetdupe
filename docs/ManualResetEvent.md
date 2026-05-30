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

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

ManualResetEvent mre(false);

Thread t([&mre]() {
    mre.WaitOne();
    // Proceed after signal
});
t.Start();

mre.Set();
```

