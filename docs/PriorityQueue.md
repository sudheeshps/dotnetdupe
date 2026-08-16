# PriorityQueue&lt;TElement, TPriority&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/PriorityQueue.h"`

Represents a collection of items that have a value and a priority. On dequeue, the item with the lowest priority value is removed (Min-Heap behavior).

---

## Syntax

```cpp
template <typename TElement, typename TPriority>
class PriorityQueue : public Object;
```

---

## Constructors

### `PriorityQueue()`
Initializes a new, empty instance of the `PriorityQueue<TElement, TPriority>` class.

---

## Properties

### `int GetCount() const`
Gets the number of elements contained in the `PriorityQueue<TElement, TPriority>`.

---

## Member Functions

### `void Enqueue(const TElement& element, const TPriority& priority)`
Adds the specified element with associated priority to the `PriorityQueue<TElement, TPriority>`.

### `TElement Dequeue()`
Removes and returns the minimal element from the `PriorityQueue<TElement, TPriority>`.
- **Throws:**
  - `InvalidOperationException`: If the queue is empty.

### `TElement Peek() const`
Returns the minimal element without removing it.
- **Throws:**
  - `InvalidOperationException`: If the queue is empty.

### `bool TryDequeue(TElement& element, TPriority& priority)`
Removes the minimal element, copying both the element and its priority to out parameters.
- **Returns:**
  - `bool`: `true` if an element was removed; `false` if the priority queue is empty.

### `void Clear()`
Removes all items from the `PriorityQueue<TElement, TPriority>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/PriorityQueue.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    PriorityQueue<String, int> tasks;
    tasks.Enqueue("Low priority background task", 3);
    tasks.Enqueue("Critical system alert", 1);
    tasks.Enqueue("Standard user request", 2);

    Console::WriteLine("Highest priority task: {0}", tasks.Peek());

    String item;
    int priority;
    while (tasks.TryDequeue(item, priority)) {
        Console::WriteLine("[Priority {0}] Processed: {1}", priority, item);
    }

    return 0;
}
```
