### template class `Array<T>`

Provides methods for creating, manipulating, searching, and sorting arrays.

#### Methods

##### `Array(int length)`

Initializes a new instance of the `Array` class with the specified length.

**Parameters:**
- `length`: The number of elements that the new `Array` can contain.

**Usage:**
```cpp
Array<int> arr(10);
```

##### `Array(const std::initializer_list<T>& items)`

Initializes a new instance of the `Array` class that contains elements copied from the specified initializer list.

**Parameters:**
- `items`: The initializer list whose elements are copied to the new `Array`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3, 4, 5 };
```

##### `int GetLength() const`

Gets the total number of elements in the `Array`.

**Returns:**
- The total number of elements in the `Array`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
int len = arr.GetLength(); // 3
```

##### `T* GetData()`

Returns a pointer to the underlying data buffer.

**Returns:**
- A pointer to the first element of the `Array`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
int* pData = arr.GetData();
```

##### `bool IsNull() const`

Gets a value indicating whether the `Array` is null (empty).

**Returns:**
- `true` if the `Array` is empty; otherwise, `false`.

**Usage:**
```cpp
Array<int> arr(0);
if (arr.IsNull()) {
    // ...
}
```

##### `T& operator[](int index)`

Gets or sets the element at the specified index.

**Parameters:**
- `index`: The zero-based index of the element to get or set.

**Returns:**
- The element at the specified index.

**Usage:**
```cpp
Array<int> arr(5);
arr[0] = 10;
int val = arr[0];
```

##### `int IndexOf(const T& value) const`

Searches for the specified object and returns the index of its first occurrence in the `Array`.

**Parameters:**
- `value`: The object to locate in the `Array`.

**Returns:**
- The zero-based index of the first occurrence of `value` in the `Array`, if found; otherwise, -1.

**Usage:**
```cpp
Array<int> arr = { 10, 20, 30, 20 };
int index = arr.IndexOf(20); // 1
```

##### `int LastIndexOf(const T& value) const`

Searches for the specified object and returns the index of its last occurrence in the `Array`.

**Parameters:**
- `value`: The object to locate in the `Array`.

**Returns:**
- The zero-based index of the last occurrence of `value` in the `Array`, if found; otherwise, -1.

**Usage:**
```cpp
Array<int> arr = { 10, 20, 30, 20 };
int index = arr.LastIndexOf(20); // 3
```

##### `void Sort()`

Sorts the elements in the entire `Array`.

**Usage:**
```cpp
Array<int> arr = { 3, 1, 4, 1, 5 };
arr.Sort();
```

##### `void Reverse()`

Reverses the sequence of the elements in the entire `Array`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
arr.Reverse(); // { 3, 2, 1 }
```

##### `void Clear()`

Sets all elements in the `Array` to their default value.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
arr.Clear(); // All elements become 0
```

##### `void CopyTo(Array<T>& array, int index)`

Copies all the elements of the current `Array` to the specified `Array` starting at the specified destination index.

**Parameters:**
- `array`: The one-dimensional `Array` that is the destination of the elements copied from the current `Array`.
- `index`: The zero-based index in `array` at which copying begins.

**Usage:**
```cpp
Array<int> src = { 1, 2, 3 };
Array<int> dest(5);
src.CopyTo(dest, 1);
```

##### `static void Copy(Array<T>& sourceArray, Array<T>& destinationArray, int length)`

Copies a range of elements from a `sourceArray` and pastes them into a `destinationArray`.

**Parameters:**
- `sourceArray`: The `Array` that contains the data to copy.
- `destinationArray`: The `Array` that receives the data.
- `length`: The number of elements to copy.

**Usage:**
```cpp
Array<int> src = { 1, 2, 3, 4, 5 };
Array<int> dest(3);
Array<int>::Copy(src, dest, 3);
```

##### `bool Exists(const std::function<bool(T)>& predicate) const`

Determines whether the `Array` contains elements that match the conditions defined by the specified predicate.

**Parameters:**
- `predicate`: The function that defines the conditions of the elements to search for.

**Returns:**
- `true` if the `Array` contains one or more elements that match the conditions; otherwise, `false`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
bool exists = arr.Exists([](int x) { return x > 2; }); // true
```

##### `T Find(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence.

**Parameters:**
- `predicate`: The function that defines the conditions of the element to search for.

**Returns:**
- The first element that matches the conditions; otherwise, the default value for type `T`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
int val = arr.Find([](int x) { return x > 1; }); // 2
```

##### `Array<T> FindAll(const std::function<bool(T)>& predicate) const`

Retrieves all the elements that match the conditions defined by the specified predicate.

**Parameters:**
- `predicate`: The function that defines the conditions of the elements to search for.

**Returns:**
- An `Array` containing all the elements that match the conditions.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3, 4 };
Array<int> results = arr.FindAll([](int x) { return x % 2 == 0; }); // { 2, 4 }
```

##### `int FindIndex(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the first occurrence.

**Parameters:**
- `predicate`: The function that defines the conditions of the element to search for.

**Returns:**
- The zero-based index of the first occurrence of an element that matches the conditions, if found; otherwise, -1.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
int index = arr.FindIndex([](int x) { return x == 2; }); // 1
```

##### `T FindLast(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the last occurrence.

**Parameters:**
- `predicate`: The function that defines the conditions of the element to search for.

**Returns:**
- The last element that matches the conditions; otherwise, the default value for type `T`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3, 2 };
int val = arr.FindLast([](int x) { return x == 2; }); // 2 (at index 3)
```

##### `int FindLastIndex(const std::function<bool(T)>& predicate) const`

Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the last occurrence.

**Parameters:**
- `predicate`: The function that defines the conditions of the element to search for.

**Returns:**
- The zero-based index of the last occurrence of an element that matches the conditions, if found; otherwise, -1.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3, 2 };
int index = arr.FindLastIndex([](int x) { return x == 2; }); // 3
```

##### `void ForEach(const std::function<void(T)>& action)`

Performs the specified action on each element of the `Array`.

**Parameters:**
- `action`: The action to perform on each element of the `Array`.

**Usage:**
```cpp
Array<int> arr = { 1, 2, 3 };
arr.ForEach([](int x) { std::cout << x << std::endl; });
```

##### `bool TrueForAll(const std::function<bool(T)>& predicate) const`

Determines whether every element in the `Array` matches the conditions defined by the specified predicate.

**Parameters:**
- `predicate`: The function that defines the conditions to check against the elements.

**Returns:**
- `true` if every element in the `Array` matches the conditions; otherwise, `false`.

**Usage:**
```cpp
Array<int> arr = { 2, 4, 6 };
bool allEven = arr.TrueForAll([](int x) { return x % 2 == 0; }); // true
```
