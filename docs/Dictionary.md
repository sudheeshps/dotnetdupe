### template class `Dictionary<TKey, TValue>`

Represents a collection of keys and values.

#### Methods

##### `Dictionary()`

Initializes a new instance of the `Dictionary<TKey, TValue>` class that is empty.

**Usage:**
```cpp
Dictionary<String, int> dict;
```

##### `int GetCount() const`

Gets the number of key/value pairs contained in the `Dictionary`.

**Usage:**
```cpp
int count = dict.GetCount();
```

##### `void Add(const TKey& key, const TValue& value)`

Adds the specified key and value to the dictionary.

**Usage:**
```cpp
dict.Add("Key", 100);
```

##### `bool ContainsKey(const TKey& key) const`

Determines whether the `Dictionary` contains the specified key.

**Usage:**
```cpp
if (dict.ContainsKey("Key")) { /* ... */ }
```

##### `bool Remove(const TKey& key)`

Removes the value with the specified key from the `Dictionary`.

**Usage:**
```cpp
bool removed = dict.Remove("Key");
```

##### `bool TryGetValue(const TKey& key, TValue& value) const`

Gets the value associated with the specified key.

**Usage:**
```cpp
int val;
if (dict.TryGetValue("Key", val)) { /* ... */ }
```

##### `TValue& operator[](const TKey& key)`

Gets or sets the value associated with the specified key.

**Usage:**
```cpp
dict["Key"] = 42;
int val = dict["Key"];
```

##### `void Clear()`

Removes all keys and values from the `Dictionary`.

**Usage:**
```cpp
dict.Clear();
```

### Iteration
`Dictionary` supports range-based for loops. Using C++17 structured bindings is recommended.

**Usage:**
```cpp
for (auto const& [key, val] : dict) {
    // ...
}
```

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `Dictionary`.

```cpp
#include "System/Collections/Generic/Dictionary.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Console.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    try {
        // Create a Dictionary mapping keys to dynamically managed values using SmartPointer
        Dictionary<String, SmartPointer<String>> dict;

        // Add elements
        dict.Add("one", SmartPointer<String>::New("First"));
        dict.Add("two", SmartPointer<String>::New("Second"));
        dict.Add("three", SmartPointer<String>::New("Third"));

        Console::WriteLine("Dictionary contents:");
        for (const auto& [key, pStr] : dict) {
            Console::Write(key);
            Console::Write(" -> ");
            if (!pStr.IsNull()) {
                Console::WriteLine(*pStr);
            } else {
                Console::WriteLine("null");
            }
        }

        // Search for a key
        if (dict.ContainsKey("two")) {
            auto val = dict["two"];
            if (!val.IsNull()) {
                Console::Write("Found value for key 'two': ");
                Console::WriteLine(*val);
            }
        }
    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


