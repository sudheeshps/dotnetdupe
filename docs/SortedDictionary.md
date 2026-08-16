# SortedDictionary&lt;TKey, TValue&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/SortedDictionary.h"`

Represents a collection of key/value pairs that are sorted on the key according to the key's comparison operator ($O(\log N)$ search via binary search).

---

## Syntax

```cpp
template <typename TKey, typename TValue>
class SortedDictionary : public Object;
```

---

## Constructors

### `SortedDictionary()`
Initializes a new, empty instance of the `SortedDictionary<TKey, TValue>` class.

---

## Properties

### `int GetCount() const`
Gets the total number of key/value pairs contained in the `SortedDictionary<TKey, TValue>`.

### `TValue& operator[](const TKey& key)`
Gets or sets the value associated with the specified key. If the key does not exist, inserts a new entry in sorted order.

### `const TValue& operator[](const TKey& key) const`
Gets the value associated with the specified key.
- **Throws:**
  - `ArgumentException`: If the key is not found.

---

## Member Functions

### `void Add(const TKey& key, const TValue& value)`
Adds an element with the specified key and value to the `SortedDictionary<TKey, TValue>`.
- **Throws:**
  - `ArgumentException`: If an element with the same key already exists.

### `bool Remove(const TKey& key)`
Removes the element with the specified key from the `SortedDictionary<TKey, TValue>`.
- **Returns:**
  - `bool`: `true` if the element is successfully found and removed; otherwise, `false`.

### `bool ContainsKey(const TKey& key) const`
Determines whether the `SortedDictionary<TKey, TValue>` contains an element with the specified key.

### `bool TryGetValue(const TKey& key, TValue& value) const`
Gets the value associated with the specified key.
- **Returns:**
  - `bool`: `true` if the key was found; otherwise, `false`.

### `void Clear()`
Removes all elements from the `SortedDictionary<TKey, TValue>`.

### `Array<TKey> GetKeys() const`
Gets an array containing all keys in sorted order.

### `Array<TValue> GetValues() const`
Gets an array containing all values sorted according to their corresponding keys.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/SortedDictionary.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    SortedDictionary<int, String> errorCodes;
    errorCodes.Add(500, "Internal Server Error");
    errorCodes.Add(200, "OK");
    errorCodes.Add(404, "Not Found");

    Console::WriteLine("Total mapped codes: {0}", errorCodes.GetCount());

    Array<int> keys = errorCodes.GetKeys();
    Console::WriteLine("Keys in sorted order:");
    for (int i = 0; i < keys.GetLength(); ++i) {
        Console::WriteLine(" - {0}: {1}", keys[i], errorCodes[keys[i]]);
    }

    return 0;
}
```
