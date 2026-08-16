# HashSet&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/HashSet.h"`

Represents a set of unique elements backed by a high-performance hash table. Provides set algebra operations including union, intersection, and symmetric difference.

---

## Syntax

```cpp
template <typename T>
class HashSet : public Object;
```

---

## Constructors & Assignment

### `HashSet()`
Initializes a new, empty instance of the `HashSet<T>` class.

### `HashSet(const HashSet& other)` / `HashSet(HashSet&& other) noexcept`
Copy and move constructors.

### `HashSet& operator=(const HashSet& other)` / `operator=(HashSet&& other) noexcept`
Copy and move assignment operators.

---

## Properties

### `int GetCount() const`
Gets the total number of elements contained in the `HashSet<T>`.

---

## Member Functions

### `bool Add(const T& item)`
Adds the specified element to the set.
- **Returns:**
  - `bool`: `true` if the element is added to the set; `false` if the element is already present.

### `bool Remove(const T& item)`
Removes the specified element from a `HashSet<T>` object.
- **Returns:**
  - `bool`: `true` if the element is successfully found and removed; otherwise, `false`.

### `bool Contains(const T& item) const`
Determines whether a `HashSet<T>` object contains the specified element.

### `void Clear()`
Removes all elements from a `HashSet<T>` object.

### `void UnionWith(const HashSet<T>& other)`
Modifies the current `HashSet<T>` object to contain all elements that are present in itself, the specified collection, or both.

```cpp
setA.UnionWith(setB);
```

### `void IntersectWith(const HashSet<T>& other)`
Modifies the current `HashSet<T>` object to contain only elements that are present in that object and in the specified collection.

```cpp
setA.IntersectWith(setB);
```

### `void ExceptWith(const HashSet<T>& other)`
Removes all elements in the specified collection from the current `HashSet<T>` object.

### `Array<T> ToArray() const`
Copies the elements of the `HashSet<T>` object to a new array.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/HashSet.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    HashSet<String> allowedRoles;
    allowedRoles.Add("Administrator");
    allowedRoles.Add("Manager");
    allowedRoles.Add("Auditor");

    Console::WriteLine("Is 'Manager' allowed: {0}", allowedRoles.Contains("Manager"));
    Console::WriteLine("Is 'Guest' allowed:   {0}", allowedRoles.Contains("Guest"));

    HashSet<String> revokedRoles;
    revokedRoles.Add("Auditor");
    allowedRoles.ExceptWith(revokedRoles);

    Console::WriteLine("Remaining roles count: {0}", allowedRoles.GetCount());

    return 0;
}
```
