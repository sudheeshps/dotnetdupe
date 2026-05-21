# Interlocked<T>

Provides atomic operations for variables that are shared by multiple threads.

## Template Parameters

- `T`: The type of the value to perform atomic operations on. Supported sizes are 4 bytes (e.g., `long`, `int`) and 8 bytes (e.g., `long long`, `__int64`).

## Constructors

### `Interlocked(T initialValue = 0)`
Initializes a new instance of the `Interlocked` class with the specified initial value.

## Methods

### `T Increment()`
Increments the value and returns the new value, as an atomic operation.

### `T Decrement()`
Decrements the value and returns the new value, as an atomic operation.

### `T Add(T value)`
Adds a value to the current value and returns the new value, as an atomic operation.

### `T Exchange(T value)`
Sets the value to a specified value as an atomic operation and returns the original value.

### `T CompareExchange(T value, T comparand)`
Compares the current value with `comparand` for equality and, if they are equal, replaces it with `value`. Returns the original value.

## Operator Overloading

### `T operator++()`
Prefix increment operator. Calls `Increment()`.

### `T operator++(int)`
Postfix increment operator. Increments the value atomically and returns the original value.

### `T operator--()`
Prefix decrement operator. Calls `Decrement()`.

### `T operator--(int)`
Postfix decrement operator. Decrements the value atomically and returns the original value.

### `T operator+=(T value)`
Addition assignment operator. Calls `Add(value)`.

### `operator T() const`
Implicit conversion to the underlying type `T`.

### `T operator=(T value)`
Assignment operator. Calls `Exchange(value)`.

## Remarks

The `Interlocked` class provides a thread-safe way to perform common operations on shared variables without requiring explicit locks. It uses hardware-intrinsic atomic operations to ensure that the updates are performed atomically across all processors.

The implementation uses `_Interlocked` intrinsics from the Windows header, ensuring high performance and reliability on Windows platforms.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

Interlocked<int> counter(0);
counter.Increment();
counter++;
int result = counter; // result is 2
```
