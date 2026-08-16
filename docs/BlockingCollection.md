# BlockingCollection&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Concurrent`  
**Header:** `#include "System/Collections/Concurrent/BlockingCollection.h"`

Provides blocking and bounding capabilities for thread-safe collections implementing the Producer-Consumer pattern.

---

## Syntax

```cpp
template <typename T>
class BlockingCollection : public Object;
```

---

## Constructors

### `BlockingCollection()`
Initializes a new, unbounded instance of the `BlockingCollection<T>` class.

### `explicit BlockingCollection(int iBoundedCapacity)`
Initializes a new instance of the `BlockingCollection<T>` class with the specified upper-bound capacity limit.
- **Throws:**
  - `ArgumentException`: If `iBoundedCapacity <= 0`.

---

## Properties

### `int GetCount() const`
Gets the number of items contained in the `BlockingCollection<T>`.

### `int GetBoundedCapacity() const`
Gets the bounded capacity of this `BlockingCollection<T>` instance (`-1` if unbounded).

### `bool IsAddingCompleted() const`
Gets whether this `BlockingCollection<T>` has been marked as complete for adding.

### `bool IsCompleted() const`
Gets whether this `BlockingCollection<T>` has been marked as complete for adding and is empty.

---

## Producer Operations (Add)

### `void Add(const T& item)`
Adds the item to the `BlockingCollection<T>`. Blocks if the collection is bounded and full until space is available.
- **Throws:**
  - `InvalidOperationException`: If the collection has been marked as complete for adding.

### `bool TryAdd(const T& item, int iTimeoutMilliseconds = 0)`
Attempts to add the specified item to the `BlockingCollection<T>` within the specified time period.
- **Returns:**
  - `bool`: `true` if the item could be added; otherwise, `false`.

---

## Consumer Operations (Take)

### `T Take()`
Removes and returns an item from the `BlockingCollection<T>`. Blocks until an item is available.
- **Returns:**
  - `T`: The item removed from the collection.
- **Throws:**
  - `InvalidOperationException`: If adding was completed and the collection is empty.

### `bool TryTake(T& item, int iTimeoutMilliseconds = 0)`
Attempts to remove an item from the `BlockingCollection<T>` within the specified time limit.
- **Returns:**
  - `bool`: `true` if an item was removed; otherwise, `false`.

```cpp
String data;
if (buffer.TryTake(data, 1000)) { // wait up to 1 second
    Console::WriteLine("Consumed: {0}", data);
}
```

---

## Completion & Utilities

### `void CompleteAdding()`
Marks the `BlockingCollection<T>` instances as not accepting any more additions. Wakes up waiting consumers and producers.

### `Array<T> ToArray() const`
Copies the items from the `BlockingCollection<T>` instance into a new `Array<T>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/BlockingCollection.h"
#include "System/Threading/ThreadPool.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

int main() {
    // Bounded capacity of 3 items
    BlockingCollection<int> pipeline(3);

    // Producer thread
    ThreadPool::QueueUserWorkItem([&pipeline]() {
        for (int i = 1; i <= 5; ++i) {
            pipeline.Add(i * 10);
            Console::WriteLine("Produced: {0}", i * 10);
        }
        pipeline.CompleteAdding();
    });

    // Consumer thread
    while (!pipeline.IsCompleted()) {
        int item;
        if (pipeline.TryTake(item, 500)) {
            Console::WriteLine("Consumed: {0}", item);
        }
    }

    Console::WriteLine("Pipeline processing complete.");
    return 0;
}
```
