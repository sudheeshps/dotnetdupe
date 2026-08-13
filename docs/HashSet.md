# HashSet<T>

The `HashSet<T>` class provides a high-performance collection of unique elements, backed by a high-performance hash table (internally using `Dictionary<T, bool>`).

## Example Usage

```cpp
#include "System/Collections/Generic/HashSet.h"
#include "System/String.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void HashSetExample() {
    HashSet<String> allowedUsers;
    
    // Add elements
    allowedUsers.Add("Alice");
    allowedUsers.Add("Bob");
    
    // Contains
    if (allowedUsers.Contains("Alice")) {
        Console::WriteLine("Alice is allowed.");
    }
    
    // Set operations
    HashSet<String> moreUsers;
    moreUsers.Add("Charlie");
    moreUsers.Add("Alice");
    
    allowedUsers.UnionWith(moreUsers);
    
    Console::WriteLine("Total allowed users: {0}", allowedUsers.GetCount());
}
```
