### System.Net.Http Namespace

Provides a programming interface for modern HTTP applications.

---

### class `RestClient<TResource>`

Represents a strongly typed REST client that simplifies resource-oriented Web API interactions. It internally wraps `HttpClient` and automates the process of serializing request objects to JSON and deserializing response JSON payloads back to C++ objects or collections.

#### Template Parameters
- `TResource`: The type representing the REST resource (must have a registered `JsonConverter` specialization).

#### Constructor

##### `RestClient(const String& baseUrl)`
Initializes a new instance of the `RestClient` class with the base URL for the resource endpoints (e.g. `"http://127.0.0.1:5000/api/products"`).

#### Methods

##### `Collections::Generic::List<TResource> GetAll()`
Sends a GET request to the base URL and deserializes the JSON array response into a list of resource objects.

##### `TResource Get(const String& id)`
Sends a GET request to the base URL with the ID appended (e.g. `/api/resource/{id}`) and deserializes the JSON object response into a single resource object.

##### `String Post(const TResource& resource)`
Sends a POST request to the base URL containing the serialized resource object in the body. Returns the raw response body as a string.

##### `template <typename TResult = TResource> TResult PostAndReturn(const TResource& resource)`
Sends a POST request to the base URL containing the serialized resource object. Deserializes the JSON response back to a `TResult` object and returns it.

##### `String Put(const String& id, const TResource& resource)`
Sends a PUT request to the base URL with the ID appended (e.g. `/api/resource/{id}`) containing the serialized resource object in the body. Returns the raw response body as a string.

##### `void Delete(const String& id)`
Sends a DELETE request to the base URL with the ID appended (e.g. `/api/resource/{id}`).

---

## Code Example

The following example demonstrates how to define a domain resource class (`Product`), register its `JsonConverter`, and perform CRUD operations against a Web API using the strongly typed `RestClient`.

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"
#include "System/Net/Http/RestClient.h"
#include "System/Text/Json/JsonSerializer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::Text::Json;

// 1. Define the resource structure
struct Product {
    String Name;
    int Price = 0;
};

// 2. Register a JsonConverter for the resource to enable serialization/deserialization
namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<Product> {
                    static JsonElement Write(const Product& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("name", JsonElement(value.Name));
                        obj.SetProperty("price", JsonElement(value.Price));
                        return obj;
                    }

                    static Product Read(const JsonElement& element) {
                        Product p;
                        JsonElement prop;
                        if (element.TryGetProperty("name", prop)) p.Name = prop.GetString();
                        if (element.TryGetProperty("price", prop)) p.Price = prop.GetInt32();
                        return p;
                    }
                };
            }
        }
    }
}

int main() {
    try {
        // Create a strongly-typed RestClient for Product resource
        RestClient<Product> client("http://127.0.0.1:5000/api/products");

        // 1. Add a new product (POST)
        Product newProduct{"Gaming Mouse", 60};
        String postResult = client.Post(newProduct);
        Console::WriteLine("POST Result: " + postResult);

        // 2. Fetch all products (GET)
        List<Product> products = client.GetAll();
        Console::WriteLine("Products count: " + Convert::ToString(products.GetLength()));
        for (int i = 0; i < products.GetLength(); ++i) {
            Console::WriteLine(" - " + products[i].Name + ": $" + Convert::ToString(products[i].Price));
        }

        // 3. Fetch product by ID (GET)
        Product prod1 = client.Get("1");
        Console::WriteLine("Product 1: " + prod1.Name + ", Price: $" + Convert::ToString(prod1.Price));

        // 4. Update product 1 (PUT)
        prod1.Price = 55;
        String putResult = client.Put("1", prod1);
        Console::WriteLine("PUT Result: " + putResult);

        // 5. Delete product 1 (DELETE)
        client.Delete("1");
        Console::WriteLine("Deleted product 1 successfully.");

    } catch (const BasicException<char>& ex) {
        Console::WriteLine("HTTP Error: " + String(ex.What()));
    }

    return 0;
}
```
