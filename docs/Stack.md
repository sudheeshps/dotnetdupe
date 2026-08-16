# Stack&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/Stack.h"`

Represents a variable-size Last-In-First-Out (LIFO) collection of instances of the same specified type.

---

## Syntax

```cpp
template <typename T>
class Stack : public Object;
```

---

## Constructors

### `Stack()`
Initializes a new, empty instance of the `Stack<T>` class.

---

## Properties

### `int GetCount() const`
Gets the total number of elements contained in the `Stack<T>`.

---

## Member Functions

### `void Push(const T& item)`
Inserts an object at the top of the `Stack<T>`.

### `T Pop()`
Removes and returns the object at the top of the `Stack<T>`.
- **Returns:**
  - `T`: The object removed from the top of the `Stack<T>`.
- **Throws:**
  - `InvalidOperationException`: If the stack is empty.

### `T Peek() const`
Returns the object at the top of the `Stack<T>` without removing it.
- **Throws:**
  - `InvalidOperationException`: If the stack is empty.

### `bool TryPop(T& result)`
Removes and returns a value that indicates whether there is an object at the top of the `Stack<T>`, and if one is present, copies it to `result`.
- **Returns:**
  - `bool`: `true` if an element was removed; `false` if the stack is empty.

### `bool TryPeek(T& result) const`
Returns a value that indicates whether there is an object at the top of the `Stack<T>`, and if one is present, copies it to `result`.

### `bool Contains(const T& item) const`
Determines whether an element is in the `Stack<T>`.

### `void Clear()`
Removes all objects from the `Stack<T>`.

### `Array<T> ToArray() const`
Copies the `Stack<T>` to a new array in LIFO order (top of stack becomes index 0).

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/Stack.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    Stack<String> history;
    history.Push("Page 1: Home");
    history.Push("Page 2: Dashboard");
    history.Push("Page 3: Settings");

    Console::WriteLine("Top element: {0}", history.Peek());

    String prevPage;
    while (history.TryPop(prevPage)) {
        Console::WriteLine("Navigated back from: {0}", prevPage);
    }

    return 0;
}
```
