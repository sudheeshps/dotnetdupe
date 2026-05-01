### interface `IClonable`

Defines a general-purpose mechanism for creating a new object that is a copy of the current object.

#### Methods

##### `virtual Object Clone() = 0`

Creates a new object that is a copy of the current instance.

**Returns:**
- A new object that is a copy of this instance.

**Usage:**
```cpp
class MyClass : public Object, public IClonable {
public:
    Object Clone() override {
        return MyClass(*this);
    }
};
```

---

### interface `IComparable`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Methods

##### `virtual int CompareTo(const Object& obj) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

**Parameters:**
- `obj`: An object to compare with this instance.

**Returns:**
- A value that indicates the relative order of the objects being compared. Less than zero means this instance precedes `obj`. Zero means they are equal. Greater than zero means this instance follows `obj`.

**Usage:**
```cpp
class MyComparable : public Object, public IComparable {
public:
    int Value;
    int CompareTo(const Object& obj) override {
        const MyComparable& other = static_cast<const MyComparable&>(obj);
        return Value - other.Value;
    }
};
```

---

### template interface `IComparable<T>`

Defines a method that a value type or class implements to compare itself with another object of the same type.

#### Methods

##### `virtual int CompareTo(const T& other) = 0`

Compares the current instance with another object of the same type and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.

**Parameters:**
- `other`: An object to compare with this instance.

**Returns:**
- A value that indicates the relative order of the objects being compared.

**Usage:**
```cpp
class MyInt : public IComparable<MyInt> {
public:
    int Value;
    int CompareTo(const MyInt& other) override {
        return Value - other.Value;
    }
};
```

---

### interface `IFormatProvider<T>`

Provides a mechanism for retrieving a formatting service for a specified type.

#### Methods

##### `virtual Object* GetFormat(const T* formatType) = 0`

Returns an object that provides formatting services for the specified type.

**Parameters:**
- `formatType`: An object that specifies the type of format object to return.

**Returns:**
- An instance of the object specified by `formatType`, if the `IFormatProvider` implementation can supply that type of object; otherwise, `nullptr`.

**Usage:**
```cpp
class MyFormatProvider : public IFormatProvider<String> {
public:
    Object* GetFormat(const String* formatType) override {
        // Implementation
        return nullptr;
    }
};
```
