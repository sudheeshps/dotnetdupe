# JsonSerializer &amp; JsonElement

**Namespace:** `DotNetDupe::System::Text::Json`  
**Header:** `#include "System/Text/Json/JsonSerializer.h"`

Provides high-performance functionality to serialize C++ objects to JSON strings and deserialize JSON strings into C++ objects and collections (`List<T>`, `Dictionary<String, T>`, primitives).

---

## `JsonSerializer` Class

### Syntax
```cpp
class JsonSerializer;
```

---

## Static Methods

### `template <typename T> static String Serialize(const T& value)`
Converts the value of a type specified by a generic type parameter into a JSON string.

```cpp
List<String> fruits;
fruits.Add("Apple");
fruits.Add("Mango");

String json = JsonSerializer::Serialize(fruits); // ["Apple","Mango"]
```

### `template <typename T> static T Deserialize(const String& sJson)`
Parses the text representing a single JSON value into an instance of the type specified by a generic type parameter.
- **Throws:**
  - `JsonException`: If the JSON text is invalid or cannot be converted to target type `T`.

```cpp
List<String> list = JsonSerializer::Deserialize<List<String>>("[\"Apple\",\"Mango\"]");
```

---

## Custom Struct Serialization (`JSON_SERIALIZABLE` Macro)

To make a custom C++ structure serializable, declare the `JSON_SERIALIZABLE` macro:

```cpp
struct Product {
    int id;
    String name;
    double price;
};

JSON_SERIALIZABLE(Product, id, name, price)
```

---

## Example

```cpp
#include "System/Console.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/Collections/Generic/List.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text::Json;
using namespace DotNetDupe::System::Collections::Generic;

struct Employee {
    int id;
    String name;
    String department;
};

JSON_SERIALIZABLE(Employee, id, name, department)

int main() {
    List<Employee> team;
    team.Add({ 1, "Alice", "Engineering" });
    team.Add({ 2, "Bob", "Design" });

    // Serialize
    String json = JsonSerializer::Serialize(team);
    Console::WriteLine("Serialized JSON:\n{0}", json);

    // Deserialize
    List<Employee> parsedTeam = JsonSerializer::Deserialize<List<Employee>>(json);
    Console::WriteLine("Deserialized {0} employees:", parsedTeam.GetCount());
    for (int i = 0; i < parsedTeam.GetCount(); ++i) {
        Console::WriteLine(" - {0} ({1})", parsedTeam[i].name, parsedTeam[i].department);
    }

    return 0;
}
```
