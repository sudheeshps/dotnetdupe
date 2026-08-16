# Interlocked&lt;T&gt;

**Namespace:** `DotNetDupe::System::Threading`  
**Header:** `#include "System/Threading/Interlocked.h"`

Provides atomic hardware-level operations for variables that are shared by multiple threads (32-bit and 64-bit integers).

---

## Syntax

```cpp
template <typename T>
class Interlocked : public Object;
```

---

## Constructors

### `Interlocked(T initialValue = 0)`
Initializes a new atomic variable with the given initial value.

---

## Atomic Operations

### `T Increment()`
Increments a specified variable and stores the result, as an atomic operation.
- **Returns:**
  - `T`: The incremented value.

### `T Decrement()`
Decrements a specified variable and stores the result, as an atomic operation.
- **Returns:**
  - `T`: The decremented value.

### `T Add(T value)`
Adds a value to the current variable and stores the result atomically.
- **Returns:**
  - `T`: The new total value.

### `T Exchange(T value)`
Sets the variable to a specified value and returns the original value, as an atomic operation.
- **Returns:**
  - `T`: The original value before assignment.

### `T CompareExchange(T value, T comparand)`
Compares the variable with `comparand` and, if they are equal, replaces the value with `value`.
- **Returns:**
  - `T`: The original value that was stored in the variable.

---

## Operator Overloads

- `T operator++()`: Prefix increment (`++atomicVar`).
- `T operator++(int)`: Postfix increment (`atomicVar++`).
- `T operator--()`: Prefix decrement (`--atomicVar`).
- `T operator--(int)`: Postfix decrement (`atomicVar--`).
- `T operator+=(T value)`: Atomic addition (`atomicVar += 5`).
- `operator T() const`: Implicit read conversion to type `T`.
- `T operator=(T value)`: Atomic assignment (`atomicVar = 100`).

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/Interlocked.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

Interlocked<int> g_atomicHits(0);

int main() {
    for (int i = 0; i < 10; ++i) {
        ThreadPool::QueueUserWorkItem([](Object* state) {
            g_atomicHits++;
        });
    }

    Thread::Sleep(100);

    Console::WriteLine("Total atomic hits: {0}", (int)g_atomicHits);

    int prev = g_atomicHits.CompareExchange(100, 10);
    Console::WriteLine("CompareExchange (if 10 -> 100): Prev was {0}, Now is {1}", prev, (int)g_atomicHits);

    return 0;
}
```
