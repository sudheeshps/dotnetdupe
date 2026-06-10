### class `JsonSerializer`

Provides functionality to serialize C++ objects or standard/library containers to JSON strings, and deserialize JSON strings back into C++ types using compile-time template specializations.

#### Namespace
`DotNetDupe::System::Text::Json`

---

#### Methods

##### `template <typename T> static String Serialize(const T& value)`

Serializes the specified value to a JSON string.

* **Usage:**
  ```cpp
  int val = 42;
  String json = JsonSerializer::Serialize(val); // "42"
  
  List<int> list = { 1, 2, 3 };
  String arrJson = JsonSerializer::Serialize(list); // "[1,2,3]"
  ```

##### `template <typename T> static T Deserialize(const String& sJson)`

Parses the JSON string and deserializes it into an instance of type `T`.

* **Usage:**
  ```cpp
  int val = JsonSerializer::Deserialize<int>("42");
  
  List<int> list = JsonSerializer::Deserialize<List<int>>("[1,2,3]");
  ```

---

### class `JsonElement`

Represents a specific JSON value (object, array, string, number, boolean, or null). Used internally by the serializer and can be used directly for manual JSON parsing and manipulation.

#### Methods

##### `static JsonElement Parse(const String& sJson)`

Parses a JSON string into a `JsonElement` DOM tree.

* **Usage:**
  ```cpp
  JsonElement root = JsonElement::Parse("{\"Name\": \"Alice\", \"Age\": 30}");
  ```

##### `JsonValueKind GetValueKind() const`

Gets the kind of the JSON value (e.g., `Object`, `Array`, `Number`, `String`, etc.).

##### `bool GetBoolean() const`
##### `double GetDouble() const`
##### `int GetInt32() const`
##### `long long GetInt64() const`
##### `String GetString() const`

Gets the primitive value from the element. Throws `std::runtime_error` if the element type does not match.

##### `int GetArrayLength() const`
##### `JsonElement GetArrayElement(int iIndex) const`
##### `void AddArrayElement(const JsonElement& objElement)`

Array manipulation and inspection methods.

##### `bool TryGetProperty(const String& sPropertyName, JsonElement& objValue) const`
##### `void SetProperty(const String& sPropertyName, const JsonElement& objValue)`
##### `Collections::Generic::Array<String> GetPropertyNames() const`

Object manipulation and inspection methods.

##### `String ToString() const`

Serializes the JSON DOM node and all its children back to a standard JSON string.

---

### Extending for Custom Types

To support serialization/deserialization for user-defined structs/classes, specialize the `JsonConverter<T>` template struct:

```cpp
struct Person {
    String Name;
    int Age;
};

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<Person> {
                    static JsonElement Write(const Person& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("Name", JsonConverter<String>::Write(value.Name));
                        obj.SetProperty("Age", JsonConverter<int>::Write(value.Age));
                        return obj;
                    }

                    static Person Read(const JsonElement& element) {
                        Person p;
                        JsonElement prop;
                        if (element.TryGetProperty("Name", prop)) {
                            p.Name = JsonConverter<String>::Read(prop);
                        }
                        if (element.TryGetProperty("Age", prop)) {
                            p.Age = JsonConverter<int>::Read(prop);
                        }
                        return p;
                    }
                };
            }
        }
    }
}
```
