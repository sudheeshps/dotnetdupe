# Dictionary&lt;TKey, TValue&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/Dictionary.h"`

Represents a collection of keys and values backed by a hash table. Provides fast $O(1)$ lookups, key enumeration, and value modification.

---

## Syntax

```cpp
template <typename TKey, typename TValue>
struct KeyValuePair {
    TKey Key;
    TValue Value;
};

template <typename TKey, typename TValue>
class Dictionary : public Object;
```

---

## Constructors & Assignment

### `Dictionary()`
Initializes a new, empty instance of the `Dictionary<TKey, TValue>` class.

### `Dictionary(const Dictionary& other)` / `Dictionary(Dictionary&& other) noexcept`
Copy and move constructors.

### `Dictionary& operator=(const Dictionary& other)` / `operator=(Dictionary&& other) noexcept`
Copy and move assignment operators.

---

## Properties

### `int GetCount() const`
Gets the total number of key/value pairs contained in the `Dictionary<TKey, TValue>`.

### `TValue& operator[](const TKey& key)`
Gets or sets the value associated with the specified key. If the specified key is not found, an entry is inserted with the default value of `TValue`.

### `const TValue& operator[](const TKey& key) const`
Gets the value associated with the specified key.
- **Throws:**
  - `ArgumentException`: If the key is not found in the dictionary.

---

## Member Functions

### `void Add(const TKey& key, const TValue& value)`
Adds the specified key and value to the dictionary.
- **Throws:**
  - `ArgumentException`: If an element with the same key already exists in the dictionary.

### `bool Remove(const TKey& key)`
Removes the value with the specified key from the `Dictionary<TKey, TValue>`.
- **Returns:**
  - `bool`: `true` if the element is successfully found and removed; otherwise, `false`.

### `bool ContainsKey(const TKey& key) const`
Determines whether the `Dictionary<TKey, TValue>` contains the specified key.

### `bool TryGetValue(const TKey& key, TValue& value) const`
Gets the value associated with the specified key.
- **Parameters:**
  - `key`: The key of the value to get.
  - `value`: When this method returns, contains the value associated with the specified key, if found.
- **Returns:**
  - `bool`: `true` if the `Dictionary<TKey, TValue>` contains an element with the specified key; otherwise, `false`.

```cpp
String sRole;
if (dict.TryGetValue("admin", sRole)) {
    Console::WriteLine("Admin role: {0}", sRole);
}
```

### `void Clear()`
Removes all keys and values from the `Dictionary<TKey, TValue>`.

### `Array<TKey> GetKeys() const`
Gets an array containing all keys in the `Dictionary<TKey, TValue>`.

### `Array<TValue> GetValues() const`
Gets an array containing all values in the `Dictionary<TKey, TValue>`.

### `begin()` / `end()`
Provides iterator support for range-based for loops yielding `KeyValuePair<TKey, TValue>`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    Dictionary<String, int> scores;
    scores.Add("Alice", 95);
    scores.Add("Bob", 88);
    scores["Charlie"] = 92; // Indexer insert

    Console::WriteLine("Total entries: {0}", scores.GetCount());

    int score = 0;
    if (scores.TryGetValue("Alice", score)) {
        Console::WriteLine("Alice's score: {0}", score);
    }

    Console::WriteLine("All entries:");
    for (const auto& kvp : scores) {
        Console::WriteLine(" - {0}: {1}", kvp.Key, kvp.Value);
    }

    scores.Remove("Bob");
    Console::WriteLine("Contains Bob: {0}", scores.ContainsKey("Bob"));

    return 0;
}
```
