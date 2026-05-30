# SemaphoreSlim

Represents a lightweight alternative to `Semaphore` that limits the number of threads that can access a resource or pool of resources concurrently.

## Methods

### `SemaphoreSlim(int initialCount)`
Initializes a new instance of the `SemaphoreSlim` class, specifying the initial number of requests that can be granted concurrently.

### `SemaphoreSlim(int initialCount, int maximumCount)`
Initializes a new instance of the `SemaphoreSlim` class, specifying the initial and maximum number of requests that can be granted concurrently.

### `bool WaitOne()`
Blocks the current thread until it can enter the `SemaphoreSlim`.

### `bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until it can enter the `SemaphoreSlim` or the timeout expires.

**Exceptions:**
- `TimeoutException`: Thrown if the timeout expires.

### `int Release(int releaseCount = 1)`
Releases the `SemaphoreSlim` object a specified number of times.

### `int GetCurrentCount()`
Gets the number of remaining threads that can enter the `SemaphoreSlim` object.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

SemaphoreSlim ss(3);

ss.WaitOne();
// Do work
ss.Release(); // Releases 1 unit
```

