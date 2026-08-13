# Dictionary<TKey, TValue>

The `Dictionary<TKey, TValue>` class provides a high-performance collection of keys and values, implemented using a .NET-style array-based chaining algorithm. It offers O(1) time complexity for lookups, insertions, and deletions on average.

## Example Usage

```cpp
#include "System/Collections/Generic/Dictionary.h"
#include "System/String.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void DictionaryExample() {
    Dictionary<String, int> ages;
    
    // Add elements
    ages.Add("Alice", 30);
    ages.Add("Bob", 25);
    
    // Use operator[]
    ages["Charlie"] = 35;
    
    // Check if key exists
    if (ages.ContainsKey("Alice")) {
        Console::WriteLine("Alice is {0} years old.", ages["Alice"]);
    }
    
    // TryGetValue
    int bobAge;
    if (ages.TryGetValue("Bob", bobAge)) {
        Console::WriteLine("Bob's age is {0}", bobAge);
    }
    
    // Remove element
    ages.Remove("Charlie");
}
```
