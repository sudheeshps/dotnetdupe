# Queue&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/Queue.h"`

Represents a First-In, First-Out (FIFO) collection of objects.

---

## Syntax

```cpp
template <typename T>
class Queue : public Object;
```

---

## Constructors

### `Queue()`
Initializes a new instance of the `Queue<T>` class that is empty.

---

## Properties

### `int GetCount() const`
Gets the total number of elements contained in the `Queue<T>`.

---

## Member Functions

### `void Enqueue(const T& item)`
Adds an object to the end of the `Queue<T>`.

### `T Dequeue()`
Removes and returns the object at the beginning of the `Queue<T>`.
- **Returns:**
  - `T`: The object that is removed from the beginning of the `Queue<T>`.
- **Throws:**
  - `InvalidOperationException`: If the queue is empty.

### `T Peek() const`
Returns the object at the beginning of the `Queue<T>` without removing it.
- **Throws:**
  - `InvalidOperationException`: If the queue is empty.

### `bool TryDequeue(T& result)`
Removes the object at the beginning of the `Queue<T>`, and copies it to `result`.
- **Returns:**
  - `bool`: `true` if an element was removed; `false` if the queue is empty.

```cpp
String nextJob;
if (jobQueue.TryDequeue(nextJob)) {
    Console::WriteLine("Processing: {0}", nextJob);
}
```

### `bool TryPeek(T& result) const`
Returns a value that indicates whether there is an object at the beginning of the `Queue<T>`, and if one is present, copies it to `result`.

### `bool Contains(const T& item) const`
Determines whether an element is in the `Queue<T>`.

### `void Clear()`
Removes all objects from the `Queue<T>`.

### `Array<T> ToArray() const`
Copies the `Queue<T>` elements to a new array.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/Queue.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    Queue<String> messages;
    messages.Enqueue("First Message");
    messages.Enqueue("Second Message");
    messages.Enqueue("Third Message");

    Console::WriteLine("Front element: {0}", messages.Peek());

    while (messages.GetCount() > 0) {
        Console::WriteLine("Dequeued: {0}", messages.Dequeue());
    }

    return 0;
}
```
