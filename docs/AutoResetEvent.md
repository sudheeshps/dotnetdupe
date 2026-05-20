# AutoResetEvent

Notifies a waiting thread that an event has occurred. This class cannot be inherited.

## Methods

### `AutoResetEvent(bool initialState)`
Initializes a new instance of the `AutoResetEvent` class with a Boolean value signaling whether to set the initial state to signaled.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current event receives a signal.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out.

## Remarks
When an `AutoResetEvent` is signaled, it automatically resets to nonsignaled after a single waiting thread has been released.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

AutoResetEvent are(false);

Thread t([&are]() {
    are.WaitOne(); // Blocks
    // Released, 'are' is now nonsignaled again
});
t.Start();

are.Set();
```
