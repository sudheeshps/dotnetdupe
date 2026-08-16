# SortedSet&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/SortedSet.h"`

Represents a collection of unique objects that is maintained in sorted order. Provides set algebra methods such as `UnionWith`, `IntersectWith`, and `ExceptWith`.

---

## Syntax

```cpp
template <typename T>
class SortedSet : public Object;
```

---

## Constructors

### `SortedSet()`
Initializes a new, empty instance of the `SortedSet<T>` class.

---

## Properties

### `int GetCount() const`
Gets the number of elements in the `SortedSet<T>`.

---

## Member Functions

### `bool Add(const T& item)`
Adds an element to the set and returns a value that indicates if it was successfully added.
- **Returns:**
  - `bool`: `true` if `item` is added to the set; `false` if the element is already present.

### `bool Remove(const T& item)`
Removes a specified item from the `SortedSet<T>`.
- **Returns:**
  - `bool`: `true` if the element is successfully found and removed; otherwise, `false`.

### `bool Contains(const T& item) const`
Determines whether the set contains a specific element.

### `void Clear()`
Removes all elements from the set.

### `void UnionWith(const SortedSet<T>& other)`
Modifies the current `SortedSet<T>` object so that it contains all elements that are present in either the current object or the specified collection.

### `void IntersectWith(const SortedSet<T>& other)`
Modifies the current `SortedSet<T>` object so that it contains only elements that are present in this object and in the specified collection.

### `void ExceptWith(const SortedSet<T>& other)`
Removes all elements in the specified collection from the current `SortedSet<T>` object.

### `Array<T> ToArray() const`
Copies the elements of the `SortedSet<T>` to a new array in sorted order.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/SortedSet.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    SortedSet<int> setA;
    setA.Add(50);
    setA.Add(10);
    setA.Add(30);

    Console::WriteLine("Set A in sorted order:");
    Array<int> arr = setA.ToArray();
    for (int i = 0; i < arr.GetLength(); ++i) {
        Console::WriteLine(" - {0}", arr[i]);
    }

    SortedSet<int> setB;
    setB.Add(30);
    setB.Add(70);

    setA.UnionWith(setB);
    Console::WriteLine("Count after Union: {0}", setA.GetCount());

    return 0;
}
```
