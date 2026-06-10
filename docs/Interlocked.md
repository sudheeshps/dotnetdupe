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

## Code Example

```cpp
#include "System/Threading/Interlocked.h"
#include "System/Threading/Thread.h"
#include "System/Console.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    Console::WriteLine("Interlocked demo started.");

    // Create a shared atomic counter using SmartPointer and Interlocked
    auto pCounter = SmartPointer<Interlocked<int>>::NewShared(0);

    // Spawn threads that increment the counter atomically without locks
    auto runIncrement = [pCounter]() {
        for (int iI = 0; iI < 1000; ++iI) {
            (*pCounter)++; // Atomic increment
        }
    };

    SmartPointer<Thread> pT1 = SmartPointer<Thread>::New(runIncrement);
    SmartPointer<Thread> pT2 = SmartPointer<Thread>::New(runIncrement);

    pT1->Start();
    pT2->Start();

    pT1->Join();
    pT2->Join();

    // Verify counter value (expected to be 2000)
    int iFinalVal = *pCounter;
    Console::Write("Final counter value (expected 2000): ");
    Console::WriteLine(iFinalVal);

    // Demonstrate CompareExchange
    // If current value is 2000, exchange it with 5000
    int iOriginal = pCounter->CompareExchange(5000, 2000);
    Console::Write("Original value returned from CompareExchange: ");
    Console::WriteLine(iOriginal);
    Console::Write("New counter value after CompareExchange: ");
    Console::WriteLine((int)*pCounter);

    // Demonstrate Exchange
    int iPrevVal = pCounter->Exchange(999);
    Console::Write("Previous value returned from Exchange: ");
    Console::WriteLine(iPrevVal);
    Console::Write("New counter value after Exchange: ");
    Console::WriteLine((int)*pCounter);

    Console::WriteLine("Interlocked demo completed.");
    return 0;
}
```


