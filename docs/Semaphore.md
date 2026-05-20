# Semaphore

Limits the number of threads that can access a resource or pool of resources concurrently.

## Methods

### `Semaphore(int initialCount, int maximumCount)`
Initializes a new instance of the `Semaphore` class, specifying the initial number of entries and the maximum number of concurrent entries.

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

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

Semaphore s(2, 2);

s.WaitOne();
// Use resource
s.Release(); // Releases 1 unit
```
