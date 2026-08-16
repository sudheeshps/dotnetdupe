# ConcurrentBag&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Concurrent`  
**Header:** `#include "System/Collections/Concurrent/ConcurrentBag.h"`

Represents a thread-safe, unordered collection of objects optimized for scenarios where the same thread produces and consumes data stored in the bag.

---

## Syntax

```cpp
template <typename T>
class ConcurrentBag : public Object;
```

---

## Constructors

### `ConcurrentBag()`
Initializes a new, empty instance of the `ConcurrentBag<T>` class.

---

## Properties

### `int GetCount() const`
Gets the number of elements contained in the `ConcurrentBag<T>`.

### `bool IsEmpty() const`
Gets a value that indicates whether the `ConcurrentBag<T>` is empty.

---

## Member Functions

### `void Add(const T& item)`
Adds an object to the `ConcurrentBag<T>`.

### `bool TryTake(T& result)`
Attempts to remove and return an object from the `ConcurrentBag<T>`.
- **Returns:**
  - `bool`: `true` if an object was removed successfully; otherwise, `false`.

```cpp
int number;
if (bag.TryTake(number)) {
    Console::WriteLine("Taken from bag: {0}", number);
}
```

### `bool TryPeek(T& result) const`
Attempts to return an object from the `ConcurrentBag<T>` without removing it.

### `void Clear()`
Removes all values from the `ConcurrentBag<T>`.

### `Array<T> ToArray() const`
Copies the `ConcurrentBag<T>` elements to a new `Array<T>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/ConcurrentBag.h"
#include "System/Threading/ThreadPool.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

int main() {
    ConcurrentBag<int> bag;

    ThreadPool::QueueUserWorkItem([&bag]() {
        bag.Add(100);
        bag.Add(200);
    });

    Thread::Sleep(50);

    int val;
    while (bag.TryTake(val)) {
        Console::WriteLine("Taken: {0}", val);
    }

    return 0;
}
```
