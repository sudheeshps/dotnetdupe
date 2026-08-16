# Array&lt;T&gt;

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Array.h"`

Provides methods for creating, manipulating, searching, and sorting fixed-size contiguous memory arrays of elements with bounds-checking and C#-style algorithms.

---

## Syntax

```cpp
template <typename T>
class Array : public Object;
```

---

## Constructors

### `Array()`
Initializes an empty `Array<T>` instance of length 0.

### `explicit Array(int iLength)`
Initializes an `Array<T>` with the specified number of default-constructed elements.

```cpp
Array<int> arr(10);
```

### `Array(const std::initializer_list<T>& list)`
Initializes an `Array<T>` containing elements copied from an initializer list.

```cpp
Array<String> fruits = { "Apple", "Banana", "Cherry" };
```

---

## Properties & Indexing

### `int GetLength() const`
Gets the total number of elements in all the dimensions of the `Array<T>`.

### `T& operator[](int iIndex)` / `const T& operator[](int iIndex) const`
Gets or sets the element at the specified index.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `iIndex < 0` or `iIndex >= GetLength()`.

```cpp
fruits[0] = "Avocado";
```

### `T* GetData()` / `const T* GetData() const`
Returns a pointer to the contiguous buffer of elements.

---

## Static Methods

### `static void Copy(const Array<T>& sourceArray, int sourceIndex, Array<T>& destinationArray, int destinationIndex, int length)`
Copies a range of elements from an `Array` starting at the specified source index and pastes them to another `Array` starting at the specified destination index.

```cpp
Array<int> src = { 1, 2, 3 };
Array<int> dst(3);
Array<int>::Copy(src, 0, dst, 0, 3);
```

### `static void Clear(Array<T>& array, int index, int length)`
Sets a range of elements in an array to default values.

### `static void Reverse(Array<T>& array)` / `static void Reverse(Array<T>& array, int index, int length)`
Reverses the sequence of the elements in the entire array or a sub-range.

```cpp
Array<int>::Reverse(dst);
```

### `static void Sort(Array<T>& array)` / `static void Sort(Array<T>& array, int index, int length)`
Sorts the elements in an entire array or sub-range into ascending order.

```cpp
Array<int>::Sort(dst);
```

### `static int IndexOf(const Array<T>& array, const T& value)`
Searches for the specified object and returns the index of the first occurrence within the entire `Array`. Returns `-1` if not found.

### `static int BinarySearch(const Array<T>& array, const T& value)`
Searches a sorted `Array` for a specific element using a binary search algorithm.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Array.h"
#include "System/String.h"

using namespace DotNetDupe::System;

int main() {
    Array<int> numbers = { 42, 17, 99, 8, 23 };

    Console::WriteLine("Original Array Length: {0}", numbers.GetLength());

    Array<int>::Sort(numbers);
    Console::WriteLine("Sorted elements:");
    for (int i = 0; i < numbers.GetLength(); ++i) {
        Console::WriteLine(" - {0}", numbers[i]);
    }

    int idx = Array<int>::BinarySearch(numbers, 23);
    Console::WriteLine("Found 23 at index: {0}", idx);

    return 0;
}
```
