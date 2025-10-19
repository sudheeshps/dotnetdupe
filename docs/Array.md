### template class `Array<T>`

Provides methods for creating, manipulating, searching, and sorting arrays.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Array.h"

int main() {
    DotNetDupe::System::Array<int> arr(5);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;

    for (int i = 0; i < arr.GetLength(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

#### Methods

##### `Array(int length)`

Initializes a new instance of the `Array` class that is empty and has the specified initial capacity.

##### `Array(const std::initializer_list<T>& items)`

Initializes a new instance of the `Array` class that contains elements copied from the specified `initializer_list`.

##### `int GetLength() const`

Gets the number of elements contained in the `Array`.

##### `T* GetData()`

Returns a pointer to the underlying array.

##### `bool IsNull() const`

Gets a value indicating whether the `Array` is null.

##### `T& operator[](int index)`

Gets or sets the element at the specified index.

##### `const T& operator[](int index) const`

Gets the element at the specified index.

##### `int IndexOf(const T& value) const`

Searches for the specified object and returns the index of its first occurrence in a one-dimensional `Array`.

##### `int LastIndexOf(const T& value) const`

Searches for the specified object and returns the index of its last occurrence in a one-dimensional `Array`.

##### `void Sort()`

Sorts the elements in an entire one-dimensional `Array` using the `IComparable` implementation of each element of the `Array`.

##### `void Reverse()`

Reverses the sequence of the elements in the entire one-dimensional `Array`.

##### `void Clear()`

Sets a range of elements in the `Array` to the default value of each element type.

##### `void CopyTo(Array<T>& array, int index)`

Copies all the elements of the current one-dimensional `Array` to the specified one-dimensional `Array` starting at the specified destination `Array` index.

##### `static void Copy(Array<T>& sourceArray, Array<T>& destinationArray, int length)`

Copies a range of elements from an `Array` starting at the first element and pastes them into another `Array` starting at the first element. The length is specified as a 32-bit integer.

##### `bool Exists(const std::function<bool(T)>& predicate) const`

Determines whether the specified `Array` contains elements that match the conditions defined by the specified predicate.

##### `T Find(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence within the entire `Array`.

##### `Array<T> FindAll(const std::function<bool(T)>& predicate) const`

Retrieves all the elements that match the conditions defined by the specified predicate.

##### `int FindIndex(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the first occurrence within the entire `Array`.

##### `T FindLast(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the last occurrence within the entire `Array`.

##### `int FindLastIndex(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the last occurrence within the entire `Array`.

##### `void ForEach(const std::function<void(T)>& action)`

Performs the specified action on each element of the specified `Array`.

##### `bool TrueForAll(const std::function<bool(T)>& predicate) const`

Determines whether every element in the `Array` matches the conditions defined by the specified predicate.
