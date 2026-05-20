# Mutex

A synchronization primitive that can also be used for inter-process synchronization.

## Methods

### `Mutex()`
Initializes a new instance of the `Mutex` class with default settings.

### `bool WaitOne()`
Blocks the current thread until the current `Mutex` is acquired.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `Mutex` is acquired or the timeout expires.

**Exceptions:**
- `TimeoutException`: Thrown if the timeout expires.

### `int Release(int releaseCount = 1)`
Releases the `Mutex` once. The `releaseCount` parameter is ignored for `Mutex` but provided for compatibility with the `LockWaitHandle` base class.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

Mutex m;

m.WaitOne();
// Critical Section
m.Release(); // Releases the mutex
```
