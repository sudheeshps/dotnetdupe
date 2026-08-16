# LinkedList&lt;T&gt; &amp; LinkedListNode&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/LinkedList.h"`

Represents a doubly linked list of elements supporting $O(1)$ insertions and removals at both ends.

---

## Syntax

```cpp
template <typename T>
class LinkedListNode : public Object {
public:
    T Value;
    LinkedListNode<T>* Next;
    LinkedListNode<T>* Previous;
};

template <typename T>
class LinkedList : public Object;
```

---

## Constructors

### `LinkedList()`
Initializes a new, empty instance of the `LinkedList<T>` class.

---

## Properties

### `int GetCount() const`
Gets the total number of nodes contained in the `LinkedList<T>`.

### `LinkedListNode<T>* GetFirst() const`
Gets the first node of the `LinkedList<T>`.

### `LinkedListNode<T>* GetLast() const`
Gets the last node of the `LinkedList<T>`.

---

## Member Functions

### `LinkedListNode<T>* AddFirst(const T& value)`
Adds a new node containing the specified value at the start of the `LinkedList<T>`.

### `LinkedListNode<T>* AddLast(const T& value)`
Adds a new node containing the specified value at the end of the `LinkedList<T>`.

### `void RemoveFirst()`
Removes the node at the start of the `LinkedList<T>`.
- **Throws:**
  - `InvalidOperationException`: If the list is empty.

### `void RemoveLast()`
Removes the node at the end of the `LinkedList<T>`.
- **Throws:**
  - `InvalidOperationException`: If the list is empty.

### `bool Remove(const T& value)`
Removes the first occurrence of the specified value from the `LinkedList<T>`.
- **Returns:**
  - `bool`: `true` if the element containing `value` is successfully removed; otherwise, `false`.

### `bool Contains(const T& value) const`
Determines whether a value is in the `LinkedList<T>`.

### `void Clear()`
Removes all nodes from the `LinkedList<T>`.

### `Array<T> ToArray() const`
Copies the elements of the `LinkedList<T>` to a new `Array<T>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    LinkedList<String> train;
    train.AddLast("Engine");
    train.AddLast("Passenger Car A");
    train.AddLast("Caboose");
    train.AddFirst("Pilot");

    Console::WriteLine("Total cars: {0}", train.GetCount());

    LinkedListNode<String>* pNode = train.GetFirst();
    while (pNode != nullptr) {
        Console::WriteLine("Car: {0}", pNode->Value);
        pNode = pNode->Next;
    }

    train.RemoveFirst();
    train.RemoveLast();
    Console::WriteLine("After removing extremities, count = {0}", train.GetCount());

    return 0;
}
```
