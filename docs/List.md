### template class `List<T>`

Represents a strongly typed list of objects that can be accessed by index. Provides methods to search, sort, and manipulate lists.

#### Methods

##### `List()`

Initializes a new instance of the `List<T>` class that is empty.

**Usage:**
```cpp
List<int> list;
```

##### `List(const std::initializer_list<T>& collection)`

Initializes a new instance of the `List<T>` class that contains elements copied from the specified initializer list.

**Usage:**
```cpp
List<String> list = { "A", "B" };
```

##### `int GetCount() const`

Gets the number of elements contained in the `List<T>`.

**Usage:**
```cpp
int count = list.GetCount();
```

##### `T& operator[](int index)`

Gets or sets the element at the specified index.

**Usage:**
```cpp
list.Add(10);
int val = list[0];
```

##### `void Add(const T& item)`

Adds an object to the end of the `List<T>`.

**Usage:**
```cpp
list.Add(42);
```

##### `bool Contains(const T& item) const`

Determines whether an element is in the `List<T>`.

**Usage:**
```cpp
if (list.Contains(42)) { /* ... */ }
```

##### `int IndexOf(const T& item) const`

Searches for the specified object and returns the zero-based index of the first occurrence within the entire `List<T>`.

**Usage:**
```cpp
int index = list.IndexOf(42);
```

##### `void Insert(int index, const T& item)`

Inserts an element into the `List<T>` at the specified index.

**Usage:**
```cpp
list.Insert(0, 10);
```

##### `bool Remove(const T& item)`

Removes the first occurrence of a specific object from the `List<T>`.

**Usage:**
```cpp
bool removed = list.Remove(42);
```

##### `void RemoveAt(int index)`

Removes the element at the specified index of the `List<T>`.

**Usage:**
```cpp
list.RemoveAt(0);
```

##### `void Sort()`

Sorts the elements in the entire `List<T>` using the default comparer.

**Usage:**
```cpp
list.Sort();
```

##### `template <typename Predicate> bool Exists(Predicate match) const`

Determines whether the `List<T>` contains elements that match the conditions defined by the specified predicate.

**Usage:**
```cpp
bool exists = list.Exists([](int x) { return x > 10; });
```

##### `template <typename Predicate> T Find(Predicate match) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence within the entire `List<T>`.

**Usage:**
```cpp
int item = list.Find([](int x) { return x % 2 == 0; });
```

##### `template <typename Predicate> List<T> FindAll(Predicate match) const`

Retrieves all the elements that match the conditions defined by the specified predicate.

**Usage:**
```cpp
List<int> evens = list.FindAll([](int x) { return x % 2 == 0; });
```

##### `template <typename Predicate> bool TrueForAll(Predicate match) const`

Determines whether every element in the `List<T>` matches the conditions defined by the specified predicate.

**Usage:**
```cpp
bool allPositive = list.TrueForAll([](int x) { return x > 0; });
```

##### `Array<T> ToArray() const`

Copies the elements of the `List<T>` to a new `Array<T>`.

**Usage:**
```cpp
Array<int> arr = list.ToArray();
```

##### `void Clear()`

Removes all elements from the `List<T>`.

**Usage:**
```cpp
list.Clear();
```
### Iteration
`List<T>` supports range-based for loops.

**Usage:**
```cpp
for (const auto& item : list) {
    // ...
}
```

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `List`.

```cpp
#include "System/Collections/Generic/List.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Console.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    try {
        // Create a List of SmartPointers to demonstrate dynamic memory management
        List<SmartPointer<String>> list;
        
        // Add elements
        list.Add(SmartPointer<String>::New("Apple"));
        list.Add(SmartPointer<String>::New("Banana"));
        list.Add(SmartPointer<String>::New("Cherry"));
        
        Console::WriteLine("List contents:");
        for (const auto& pStr : list) {
            if (!pStr.IsNull()) {
                Console::WriteLine(*pStr);
            }
        }
        
        // Find an element
        auto found = list.Find([](const SmartPointer<String>& pStr) {
            return pStr->StartsWith("B", false);
        });
        
        if (!found.IsNull()) {
            Console::Write("Found string starting with B: ");
            Console::WriteLine(*found);
        }
    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


