### System.Net.Http Namespace

Provides a programming interface for modern HTTP applications.

---

### class `HttpClient`

Sends HTTP requests and receives HTTP responses from a resource identified by a URI.

#### Methods

##### `HttpClient()`
Initializes a new instance of the `HttpClient` class.

##### `SmartPointer<HttpResponseMessage> Get(const String& requestUri)`
##### `SmartPointer<HttpResponseMessage> Get(const Uri& requestUri)`
Sends a GET request to the specified Uri as an asynchronous-like blocking operation.

##### `SmartPointer<HttpResponseMessage> Post(const String& requestUri, const SmartPointer<HttpContent>& content)`
##### `SmartPointer<HttpResponseMessage> Post(const Uri& requestUri, const SmartPointer<HttpContent>& content)`
Sends a POST request to the specified Uri.

##### `SmartPointer<HttpResponseMessage> Put(const String& requestUri, const SmartPointer<HttpContent>& content)`
##### `SmartPointer<HttpResponseMessage> Put(const Uri& requestUri, const SmartPointer<HttpContent>& content)`
Sends a PUT request to the specified Uri.

##### `SmartPointer<HttpResponseMessage> Delete(const String& requestUri)`
##### `SmartPointer<HttpResponseMessage> Delete(const Uri& requestUri)`
Sends a DELETE request to the specified Uri.

##### `SmartPointer<HttpResponseMessage> Send(const SmartPointer<HttpRequestMessage>& request)`
Sends an HTTP request.

##### `String GetString(const String& requestUri)`
Sends a GET request to the specified Uri and returns the response body as a string.

##### `Array<char> GetByteArray(const String& requestUri)`
Sends a GET request to the specified Uri and returns the response body as a byte array.

##### `Dictionary<String, String>& GetDefaultRequestHeaders()`
Gets the headers which should be sent with each request.

---

### class `HttpRequestMessage`

Represents an HTTP request message.

#### Constructors
- `HttpRequestMessage()`
- `HttpRequestMessage(const HttpMethod& method, const Uri& requestUri)`
- `HttpRequestMessage(const HttpMethod& method, const String& requestUri)`

#### Methods
- `HttpMethod GetMethod() const` / `void SetMethod(const HttpMethod&)`
- `Uri GetRequestUri() const` / `void SetRequestUri(const Uri&)`
- `SmartPointer<HttpContent> GetContent() const` / `void SetContent(const SmartPointer<HttpContent>&)`
- `Dictionary<String, String>& GetHeaders()`

---

### class `HttpResponseMessage`

Represents an HTTP response message including the status code and data.

#### Methods
- `HttpStatusCode GetStatusCode() const`
- `String GetReasonPhrase() const`
- `bool IsSuccessStatusCode() const`
- `SmartPointer<HttpContent> GetContent() const`
- `Dictionary<String, String>& GetHeaders()`
- `void EnsureSuccessStatusCode()`: Throws an `HttpRequestException` if the HTTP response was unsuccessful.

---

### class `HttpContent` (Abstract)

Represents an HTTP entity body and content headers.

#### Subclasses
- `StringContent`: Provides HTTP content based on a string.
- `ByteArrayContent`: Provides HTTP content based on a byte array.

---

## Code Example

The following example demonstrates how to use `HttpClient` to send a GET request, inspect the response headers and body, handle a POST request with `StringContent`, and handle exceptions.

```cpp
#include "System/Console.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;

int main() {
    try {
        HttpClient client;

        // Add a default request header
        client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupeClient/1.0");

        Console::WriteLine("Sending GET request...");
        auto response = client.Get("http://example.com/");
        
        // Ensure success status code
        response->EnsureSuccessStatusCode();

        Console::Write("Status Code: ");
        Console::WriteLine(static_cast<int>(response->GetStatusCode()));

        auto content = response->GetContent();
        if (!content.IsNull()) {
            Console::WriteLine("Response Body:");
            Console::WriteLine(content->ReadAsString());
        }

        // POST request example
        Console::WriteLine("\nSending POST request...");
        auto postContent = SmartPointer<StringContent>::NewShared("{\"name\":\"test\"}", "application/json");
        auto postResponse = client.Post("http://example.com/api", postContent);

        if (postResponse->IsSuccessStatusCode()) {
            Console::WriteLine("POST request succeeded!");
        } else {
            Console::Write("POST failed with status: ");
            Console::WriteLine(static_cast<int>(postResponse->GetStatusCode()));
        }

    } catch (const HttpRequestException& ex) {
        Console::Write("HTTP Request Error: ");
        Console::WriteLine(ex.What());
    } catch (const BasicException<char>& ex) {
        Console::Write("General Exception: ");
        Console::WriteLine(ex.What());
    }

    return 0;
}
```
