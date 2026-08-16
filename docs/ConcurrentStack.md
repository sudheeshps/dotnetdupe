# ConcurrentStack&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Concurrent`  
**Header:** `#include "System/Collections/Concurrent/ConcurrentStack.h"`

Represents a thread-safe Last-In-First-Out (LIFO) collection that enables concurrent push and pop operations across multiple threads.

---

## Syntax

```cpp
template <typename T>
class ConcurrentStack : public Object;
```

---

## Constructors

### `ConcurrentStack()`
Initializes a new, empty instance of the `ConcurrentStack<T>` class.

---

## Properties

### `int GetCount() const`
Gets the number of elements contained in the `ConcurrentStack<T>`.

### `bool IsEmpty() const`
Gets a value that indicates whether the `ConcurrentStack<T>` is empty.

---

## Member Functions

### `void Push(const T& item)`
Inserts an object at the top of the `ConcurrentStack<T>`.

### `bool TryPop(T& result)`
Attempts to pop and return the object at the top of the `ConcurrentStack<T>`.
- **Returns:**
  - `bool`: `true` if an element was removed and returned successfully; otherwise, `false`.

### `bool TryPeek(T& result) const`
Attempts to return an object from the top of the `ConcurrentStack<T>` without removing it.

### `void Clear()`
Removes all objects from the `ConcurrentStack<T>`.

### `Array<T> ToArray() const`
Copies the elements stored in the `ConcurrentStack<T>` to a new `Array<T>` in LIFO order.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/ConcurrentStack.h"
#include "System/Threading/ThreadPool.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

int main() {
    ConcurrentStack<String> stack;

    ThreadPool::QueueUserWorkItem([&stack]() {
        stack.Push("Frame A");
        stack.Push("Frame B");
    });

    Thread::Sleep(50);

    String frame;
    while (stack.TryPop(frame)) {
        Console::WriteLine("Popped stack frame: {0}", frame);
    }

    return 0;
}
```
