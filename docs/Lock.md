# Lock<T>

The `Lock<T>` class is a template class that provides a Resource Acquisition Is Initialization (RAII) mechanism for synchronization objects. It automatically acquires the synchronization object in the constructor and releases it in the destructor.

## Template Specializations

`Lock<T>` is specialized for the following types:
- `CriticalSection`
- `Mutex`
- `Semaphore`
- `SemaphoreSlim`

## Constructors

### `Lock(T& syncObject, int millisecondsTimeout = -1, int releaseCount = -1)`

Initializes a new instance of the `Lock<T>` class and acquires the specified synchronization object.

**Parameters:**
- `syncObject`: The synchronization object to manage.
- `millisecondsTimeout`: (Optional) The number of milliseconds to wait for the lock. Default is -1 (infinite wait).
- `releaseCount`: (Optional) Specifically for `Semaphore` and `SemaphoreSlim`, the number of units to release in the destructor. Default is -1 (releases 1 unit).

**Exceptions:**
- `TimeoutException`: Thrown if `millisecondsTimeout` is exceeded before the lock is acquired.

## Destructor

### `~Lock()`

Releases the synchronization object managed by this instance.
- For `CriticalSection`: Calls `Leave()`.
- For `Mutex`, `Semaphore` and `SemaphoreSlim`: Calls `Release(releaseCount)`.

## Type Aliases

For convenience, the following type aliases are provided:
- `CriticalSectionLock`
- `MutexLock`
- `SemaphoreLock`
- `SemaphoreSlimLock`

## Example Usage

### Using MutexLock
```cpp
using namespace DotNetDupe::System::Threading;

Mutex m;

void ThreadSafeMethod() {
    MutexLock lock(m); // Mutex acquired here
    // ... thread-safe work ...
} // Mutex automatically released here
```

### Using SemaphoreLock with Timeout
```cpp
using namespace DotNetDupe::System::Threading;

Semaphore s(1, 1);

void TryWork() {
    try {
        SemaphoreLock lock(s, 100); // Wait 100ms
        // ... work ...
    } catch (const TimeoutException& ex) {
        // Handle timeout
    }
}
```

