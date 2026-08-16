# ConcurrentQueue&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Concurrent`  
**Header:** `#include "System/Collections/Concurrent/ConcurrentQueue.h"`

Represents a thread-safe First-In, First-Out (FIFO) collection that enables concurrent enqueue and dequeue operations across multiple threads.

---

## Syntax

```cpp
template <typename T>
class ConcurrentQueue : public Object;
```

---

## Constructors

### `ConcurrentQueue()`
Initializes a new, empty instance of the `ConcurrentQueue<T>` class.

---

## Properties

### `int GetCount() const`
Gets the number of elements contained in the `ConcurrentQueue<T>`.

### `bool IsEmpty() const`
Gets a value that indicates whether the `ConcurrentQueue<T>` is empty.

---

## Member Functions

### `void Enqueue(const T& item)`
Adds an object to the end of the `ConcurrentQueue<T>`.

### `bool TryDequeue(T& result)`
Attempts to remove and return the object at the beginning of the concurrent queue.
- **Returns:**
  - `bool`: `true` if an element was removed and returned successfully; otherwise, `false`.

```cpp
int value;
if (queue.TryDequeue(value)) {
    Console::WriteLine("Dequeued item: {0}", value);
}
```

### `bool TryPeek(T& result) const`
Attempts to return an object from the beginning of the `ConcurrentQueue<T>` without removing it.

### `void Clear()`
Removes all objects from the `ConcurrentQueue<T>`.

### `Array<T> ToArray() const`
Copies the elements stored in the `ConcurrentQueue<T>` to a new `Array<T>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Threading/ThreadPool.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

int main() {
    ConcurrentQueue<int> q;

    // Producer
    ThreadPool::QueueUserWorkItem([&q]() {
        for (int i = 1; i <= 5; ++i) {
            q.Enqueue(i);
        }
    });

    Thread::Sleep(50);

    // Consumer
    int item;
    while (q.TryDequeue(item)) {
        Console::WriteLine("Consumed: {0}", item);
    }

    return 0;
}
```
