# Array&lt;T&gt;

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Array.h"`

Provides methods for creating, manipulating, searching, and sorting fixed-size contiguous memory arrays of elements with bounds-checking and C#-style algorithms.

---

## Syntax

```cpp
template <class T>
class Array : public Object;
```

---

## Constructors & Destructors

### `Array()`
Initializes an empty `Array<T>` instance of length 0.

### `explicit Array(int iLength)`
Initializes an `Array<T>` with the specified number of default-constructed elements.

```cpp
Array<int> arrNumbers(10);
```

### `Array(const T* pData, int iLength)`
Initializes an `Array<T>` with `iLength` elements copied from the contiguous buffer `pData`.

### `Array(const std::initializer_list<T>& vItems)`
Initializes an `Array<T>` containing elements copied from an initializer list.

```cpp
Array<String> arrFruits = { "Apple", "Banana", "Cherry" };
```

### `Array(const Array& other)` / `Array& operator=(const Array& other)`
Copy constructor and copy assignment operator.

### `Array(Array&& other) noexcept` / `Array& operator=(Array&& other) noexcept`
Move constructor and move assignment operator.

### `~Array() override`
Destructor that cleans up allocated elements and buffers.

---

## Properties & Elements

### `int GetLength() const`
Gets the total number of elements in the `Array<T>`.

### `bool IsNull() const`
Returns `true` if the array has a length of 0.

### `T* GetData()` / `const T* GetData() const`
Returns a direct pointer to the underlying contiguous buffer.

### `T* begin()` / `T* end()` / `const T* begin() const` / `const T* end() const`
Returns iterator pointers to support range-based for loops.

### `T& operator[](int iIndex)` / `const T& operator[](int iIndex) const`
Gets or sets the element at the specified index.

---

## Searching & Predicates

### `int IndexOf(const T& value) const`
Searches for the specified value and returns the zero-based index of the first occurrence within the array. Returns `-1` if not found.

### `int LastIndexOf(const T& value) const`
Searches for the specified value and returns the zero-based index of the last occurrence within the array. Returns `-1` if not found.

### `bool Exists(const Predicate<T>& fnPredicate) const`
Determines whether the array contains elements that match the conditions defined by the specified predicate.

### `T Find(const Predicate<T>& fnPredicate) const`
Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence. Returns `T()` if not found.

### `Array<T> FindAll(const Predicate<T>& fnPredicate) const`
Retrieves all the elements that match the conditions defined by the specified predicate.

### `int FindIndex(const Predicate<T>& fnPredicate) const`
Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the first occurrence. Returns `-1` if not found.

### `T FindLast(const Predicate<T>& fnPredicate) const`
Searches for an element that matches the conditions defined by the specified predicate, and returns the last occurrence. Returns `T()` if not found.

### `int FindLastIndex(const Predicate<T>& fnPredicate) const`
Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the last occurrence. Returns `-1` if not found.

### `bool TrueForAll(const Predicate<T>& fnPredicate) const`
Determines whether every element in the array matches the conditions defined by the specified predicate.

---

## Operations & Transformations

### `void Sort()`
Sorts the elements in the array in ascending order using their `operator>`.

### `void Reverse()`
Reverses the order of the elements in the entire array.

### `void Clear()`
Resets each element in the array to its default value `T()`.

### `void ForEach(const Action<T>& fnAction)`
Performs the specified action on each element of the array.

### `void CopyTo(Array<T>& arrTarget, int iIndex)`
Copies all elements of the current array into the target array starting at the specified destination index.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `iIndex < 0`.
  - `ArgumentException`: If the destination array is not large enough.

### `static void Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength)`
Copies a specified number of elements from the source array to the destination array.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `iLength < 0`.
  - `ArgumentException`: If the source or destination array is not large enough.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Array.h"
#include "System/String.h"

using namespace DotNetDupe::System;

int main() {
    Array<int> arrNumbers = { 42, 17, 99, 8, 23 };

    Console::WriteLine("Original Array Length: {0}", arrNumbers.GetLength());

    arrNumbers.Sort();
    Console::WriteLine("Sorted elements:");
    for (int iIdx = 0; iIdx < arrNumbers.GetLength(); ++iIdx) {
        Console::WriteLine(" - {0}", arrNumbers[iIdx]);
    }

    int iFoundIndex = arrNumbers.IndexOf(23);
    Console::WriteLine("Found 23 at index: {0}", iFoundIndex);

    int iFirstEven = arrNumbers.Find([](int iVal) { return iVal % 2 == 0; });
    Console::WriteLine("First even number: {0}", iFirstEven);

    return 0;
}
```
