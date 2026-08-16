# ConcurrentDictionary&lt;TKey, TValue&gt;

**Namespace:** `DotNetDupe::System::Collections::Concurrent`  
**Header:** `#include "System/Collections/Concurrent/ConcurrentDictionary.h"`

Represents a thread-safe collection of key/value pairs that can be accessed by multiple threads concurrently without manual locking.

---

## Syntax

```cpp
template <typename TKey, typename TValue>
class ConcurrentDictionary : public Object;
```

---

## Constructors

### `ConcurrentDictionary()`
Initializes a new, empty instance of the `ConcurrentDictionary<TKey, TValue>` class.

---

## Properties

### `int GetCount() const`
Gets the number of key/value pairs contained in the `ConcurrentDictionary<TKey, TValue>`.

### `bool IsEmpty() const`
Gets a value that indicates whether the `ConcurrentDictionary<TKey, TValue>` is empty.

### `TValue& operator[](const TKey& key)`
Gets or sets the value associated with the specified key.

---

## Thread-Safe Operations

### `bool TryAdd(const TKey& key, const TValue& value)`
Attempts to add the specified key and value to the `ConcurrentDictionary<TKey, TValue>`.
- **Returns:**
  - `bool`: `true` if the key/value pair was added successfully; `false` if the key already exists.

### `bool TryGetValue(const TKey& key, TValue& value) const`
Attempts to get the value associated with the specified key from the `ConcurrentDictionary<TKey, TValue>`.
- **Returns:**
  - `bool`: `true` if the key was found; otherwise, `false`.

### `bool TryRemove(const TKey& key, TValue& value)`
Attempts to remove and return the value that has the specified key from the `ConcurrentDictionary<TKey, TValue>`.
- **Returns:**
  - `bool`: `true` if the object was removed successfully; otherwise, `false`.

### `bool ContainsKey(const TKey& key) const`
Determines whether the `ConcurrentDictionary<TKey, TValue>` contains the specified key.

### `TValue GetOrAdd(const TKey& key, const TValue& value)`
Adds a key/value pair to the `ConcurrentDictionary<TKey, TValue>` if the key does not already exist. Returns the new value, or the existing value if the key exists.

### `template <typename F> TValue GetOrAdd(const TKey& key, F valueFactory)`
Adds a key/value pair to the `ConcurrentDictionary<TKey, TValue>` by using the specified factory function if the key does not already exist.

```cpp
int counter = cache.GetOrAdd("active_users", [](const String& k) { return 1; });
```

### `TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue)`
Adds a key/value pair if the key does not exist, or updates the key/value pair if the key exists.

### `void Clear()`
Removes all keys and values from the `ConcurrentDictionary<TKey, TValue>`.

### `Array<TKey> GetKeys() const`
Gets a snapshot array containing the keys in the dictionary.

### `Array<TValue> GetValues() const`
Gets a snapshot array containing the values in the dictionary.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/Threading/ThreadPool.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

int main() {
    ConcurrentDictionary<String, int> wordCounts;

    ThreadPool::QueueUserWorkItem([&wordCounts]() {
        wordCounts.AddOrUpdate("apple", 1, 2);
    });

    ThreadPool::QueueUserWorkItem([&wordCounts]() {
        wordCounts.GetOrAdd("banana", 5);
    });

    Thread::Sleep(100);

    int count = 0;
    if (wordCounts.TryGetValue("banana", count)) {
        Console::WriteLine("Banana count: {0}", count);
    }

    return 0;
}
```
