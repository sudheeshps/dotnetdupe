### class `Object`

Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. This is the ultimate base class of all classes in the .NET Framework; it is the root of the type hierarchy.

#### Methods

##### `bool operator == (const Object& obj) const`

Determines whether the specified `Object` is equal to the current `Object`.

**Parameters:**
- `obj`: The object to compare with the current object.

**Returns:**
- `true` if the specified object is equal to the current object; otherwise, `false`.

**Usage:**
```cpp
Object obj1, obj2;
bool isEqual = (obj1 == obj2);
```

##### `virtual bool Equals(const Object& obj)`

Determines whether the specified `Object` is equal to the current `Object`.

**Parameters:**
- `obj`: The object to compare with the current object.

**Returns:**
- `true` if the specified object is equal to the current object; otherwise, `false`.

**Usage:**
```cpp
Object obj1, obj2;
bool result = obj1.Equals(obj2);
```

##### `static bool Equals(const Object& obj1, const Object& obj2)`

Determines whether the specified `Object` instances are considered equal.

**Parameters:**
- `obj1`: The first object to compare.
- `obj2`: The second object to compare.

**Returns:**
- `true` if the objects are considered equal; otherwise, `false`.

**Usage:**
```cpp
Object obj1, obj2;
bool result = Object::Equals(obj1, obj2);
```

##### `int GetHashCode()`

Serves as the default hash function.

**Returns:**
- A hash code for the current object.

**Usage:**
```cpp
Object obj;
int hash = obj.GetHashCode();
```

##### `wchar_t* ToStringW()`

Returns a string that represents the current object (wide character version).

**Returns:**
- A wide character string that represents the current object.

**Usage:**
```cpp
Object obj;
wchar_t* str = obj.ToStringW();
```

##### `char* ToStringA()`

Returns a string that represents the current object (ASCII character version).

**Returns:**
- An ASCII string that represents the current object.

**Usage:**
```cpp
Object obj;
char* str = obj.ToStringA();
```
