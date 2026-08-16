# List&lt;T&gt;

**Namespace:** `DotNetDupe::System::Collections::Generic`  
**Header:** `#include "System/Collections/Generic/List.h"`

Represents a strongly typed, dynamically-sized list of objects that can be accessed by index. Provides methods to search, sort, filter, and manipulate lists.

---

## Syntax

```cpp
template <typename T>
class List : public Object;
```

---

## Constructors & Assignment

### `List()`
Initializes a new instance of the `List<T>` class that is empty and has default initial capacity.

### `explicit List(int iCapacity)`
Initializes a new instance of the `List<T>` class that is empty and has the specified initial capacity.

### `List(const std::initializer_list<T>& vCollection)`
Initializes a `List<T>` populated with elements from an initializer list.

```cpp
List<int> numbers = { 10, 20, 30, 40 };
```

### `List(const List& lstOther)` / `List(List&& lstOther) noexcept`
Copy and move constructors.

### `List& operator=(const List& lstOther)` / `operator=(List&& lstOther) noexcept`
Copy and move assignment operators.

---

## Properties

### `int GetCount() const`
Gets the total number of elements contained in the `List<T>`.

### `int GetCapacity() const`
Gets the total number of elements the internal data structure can hold without resizing.

### `void SetCapacity(int iValue)`
Sets the total number of elements the internal data structure can hold without resizing.

### `T& operator[](int iIndex)` / `const T& operator[](int iIndex) const`
Gets or sets the element at the specified index.

---

## Adding & Modifying Elements

### `void Add(const T& item)`
Adds an object to the end of the `List<T>`. Automatically expands internal buffer when required.

### `void AddRange(const Array<T>& arrCollection)`
Adds the elements of the specified array to the end of the `List<T>`.

### `void Insert(int iIndex, const T& item)`
Inserts an element into the `List<T>` at the specified index.

### `bool Remove(const T& item)`
Removes the first occurrence of a specific object from the `List<T>`. Returns `true` if item was successfully removed; otherwise `false`.

### `void RemoveAt(int iIndex)`
Removes the element at the specified index of the `List<T>`.

### `void Clear()`
Removes all elements from the `List<T>`.

---

## Searching & Sorting

### `bool Contains(const T& item) const`
Determines whether an element is in the `List<T>`.

### `int IndexOf(const T& item) const`
Searches for the specified object and returns the zero-based index of the first occurrence within the entire `List<T>`. Returns `-1` if not found.

### `int BinarySearch(const T& item) const`
Searches the entire sorted `List<T>` for an element using a binary search algorithm.

### `void Sort()`
Sorts the elements in the entire `List<T>`.

---

## Functional & Predicate Queries

### `template <typename Predicate> bool Exists(Predicate fnMatch) const`
Determines whether the `List<T>` contains elements that match the conditions defined by the specified predicate lambda.

### `template <typename Predicate> T Find(Predicate fnMatch) const`
Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence.

### `template <typename Predicate> List<T> FindAll(Predicate fnMatch) const`
Retrieves all the elements that match the conditions defined by the specified predicate.

```cpp
auto evens = numbers.FindAll([](int n) { return n % 2 == 0; });
```

### `template <typename Predicate> bool TrueForAll(Predicate fnMatch) const`
Determines whether every element in the `List<T>` matches the conditions defined by the specified predicate.

---

## Transformations & Iteration

### `Array<T> ToArray() const`
Copies the elements of the `List<T>` to a new `Array<T>`.

### `begin()` / `end()`
Provides standard C++ iterator support enabling range-based `for (const auto& item : list)` loops.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Collections/Generic/List.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

int main() {
    List<String> cities = { "London", "Tokyo", "Paris", "New York" };
    cities.Add("San Francisco");

    Console::WriteLine("Total cities: {0}", cities.GetCount());

    // Filter using FindAll lambda
    auto shortNames = cities.FindAll([](const String& s) { return s.GetLength() <= 5; });
    Console::WriteLine("Short city names:");
    for (const auto& city : shortNames) {
        Console::WriteLine(" - {0}", city);
    }

    cities.Sort();
    Console::WriteLine("BinarySearch 'Paris': index {0}", cities.BinarySearch("Paris"));

    return 0;
}
```
