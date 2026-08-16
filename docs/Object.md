# Object

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Object.h"`

Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. This is the ultimate base class of all classes in the DotNetDupe library; it is the root of the type hierarchy.

---

## Syntax

```cpp
class Object;
```

---

## Member Functions

### `virtual ~Object() = default`
Destructor. Ensures proper polymorphic destruction of derived instances.

---

### `bool operator==(const Object& obj) const`
Determines whether two `Object` instances are equal by delegating to `Equals()`.

- **Parameters:**
  - `obj` (`const Object&`): The object to compare with the current object.
- **Returns:**
  - `bool`: `true` if the specified object is equal to the current object; otherwise, `false`.

```cpp
Object objA;
Object objB;
bool bEqual = (objA == objB);
```

---

### `virtual bool Equals(const Object& obj) const`
Determines whether the specified object is equal to the current object (reference equality by default).

- **Parameters:**
  - `obj` (`const Object&`): The object to compare with the current object.
- **Returns:**
  - `bool`: `true` if the specified object is equal to the current object; otherwise, `false`.
- **Exceptions:**
  - None.

```cpp
bool bEqual = objA.Equals(objB);
```

---

### `static bool Equals(const Object& obj1, const Object& obj2)`
Determines whether the specified object instances are considered equal.

- **Parameters:**
  - `obj1` (`const Object&`): The first object to compare.
  - `obj2` (`const Object&`): The second object to compare.
- **Returns:**
  - `bool`: `true` if the objects are considered equal; otherwise, `false`.

```cpp
bool bEqual = Object::Equals(objA, objB);
```

---

### `virtual int GetHashCode() const`
Serves as the default hash function. Computes a hash code based on the object instance address.

- **Returns:**
  - `int`: A hash code for the current object.

```cpp
int iHash = objA.GetHashCode();
```

---

### `char* ToStringA() const`
Returns a string that represents the current object as an ANSI/UTF-8 character buffer. Caller is responsible for lifetime or converting to `DotNetDupe::System::String`.

- **Returns:**
  - `char*`: A null-terminated C-string containing the type name and address.

---

### `wchar_t* ToStringW() const`
Returns a string that represents the current object as a wide character buffer.

- **Returns:**
  - `wchar_t*`: A null-terminated wide C-string containing the type name and address.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Object.h"

using namespace DotNetDupe::System;

class Person : public Object {
private:
    int m_iId;

public:
    explicit Person(int iId) : m_iId(iId) {}

    bool Equals(const Object& obj) const override {
        const Person* pOther = dynamic_cast<const Person*>(&obj);
        if (pOther == nullptr) {
            return false;
        }
        return m_iId == pOther->m_iId;
    }

    int GetHashCode() const override {
        return m_iId;
    }
};

int main() {
    Person p1(101);
    Person p2(101);
    Person p3(202);

    Console::WriteLine("p1 == p2: {0}", p1.Equals(p2)); // true
    Console::WriteLine("p1 == p3: {0}", p1.Equals(p3)); // false
    Console::WriteLine("p1 HashCode: {0}", p1.GetHashCode());

    return 0;
}
```
