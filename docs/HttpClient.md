# HttpClient, HttpRequestMessage, HttpResponseMessage &amp; HttpContent

**Namespace:** `DotNetDupe::System::Net::Http`  
**Header:** `#include "System/Net/Http/HttpClient.h"`, `#include "System/Net/Http/HttpRequestMessage.h"`, `#include "System/Net/Http/HttpResponseMessage.h"`, `#include "System/Net/Http/HttpContent.h"`, `#include "System/Net/Http/HttpMethod.h"`

Provides a fluent class for sending HTTP/HTTPS requests and receiving HTTP responses from a resource identified by a URI.

---

## `HttpMethod` Class

Static instances:
- `HttpMethod::Get`, `HttpMethod::Post`, `HttpMethod::Put`, `HttpMethod::Delete`, `HttpMethod::Head`, `HttpMethod::Options`, `HttpMethod::Patch`, `HttpMethod::Trace`.

---

## `HttpContent` (Abstract Base) & Subclasses

### Concrete Implementations
- `StringContent(const String& content, const String& mediaType = "text/plain")`
- `ByteArrayContent(const Array<char>& content, int offset = 0, int count = -1)`
- `StreamContent(const SmartPointer<IO::Stream>& stream)`

### Member Functions
- `String ReadAsString()`: Serializes HTTP content to a string.
- `Array<char> ReadAsByteArray()`: Serializes HTTP content to a byte array.
- `SmartPointer<IO::Stream> ReadAsStream()`: Serializes the HTTP content and returns a stream that represents the content.
- `void CopyTo(const SmartPointer<IO::Stream>& stream)`: Writes content into a destination stream.
- `Dictionary<String, String>& GetHeaders()`: Content headers (e.g. `Content-Type`, `Content-Length`).

---

## `HttpRequestMessage` Class

### Constructors
- `HttpRequestMessage(const HttpMethod& method, const String& requestUri)`
- `HttpRequestMessage(const HttpMethod& method, const Uri& requestUri)`

### Properties
- `HttpMethod GetMethod() const` / `void SetMethod(const HttpMethod& method)`
- `Uri GetRequestUri() const` / `void SetRequestUri(const Uri& requestUri)`
- `HttpContentPtr GetContent() const` / `void SetContent(const HttpContentPtr& content)`
- `Dictionary<String, String>& GetHeaders()`: Request headers (e.g. `Authorization`, `Accept`, `User-Agent`).

---

## `HttpResponseMessage` Class

### Properties
- `HttpStatusCode GetStatusCode() const` / `void SetStatusCode(HttpStatusCode statusCode)`
- `String GetReasonPhrase() const` / `void SetReasonPhrase(const String& reasonPhrase)`
- `bool IsSuccessStatusCode() const`: Returns `true` if HTTP status code is in 200–299 range.
- `void EnsureSuccessStatusCode()`: Throws `HttpRequestException` if the HTTP response was unsuccessful.
- `HttpContentPtr GetContent() const` / `void SetContent(const HttpContentPtr& content)`
- `Dictionary<String, String>& GetHeaders()`: Response headers dictionary.

---

## `HttpClient` Class

### Syntax
```cpp
class HttpClient : public Object;
```

### Member Functions
- `HttpResponseMessagePtr Get(const String& requestUri)`: Sends a GET request.
- `HttpResponseMessagePtr Post(const String& requestUri, const HttpContentPtr& content)`: Sends a POST request with payload.
- `HttpResponseMessagePtr Put(const String& requestUri, const HttpContentPtr& content)`: Sends a PUT request with payload.
- `HttpResponseMessagePtr Delete(const String& requestUri)`: Sends a DELETE request.
- `HttpResponseMessagePtr Send(const HttpRequestMessagePtr& request)`: Sends an HTTP request message.
- `String GetString(const String& requestUri)`: Sends a GET request and returns the response body as a `String`.
- `Array<char> GetByteArray(const String& requestUri)`: Sends a GET request and returns the response body as a byte array.
- `Dictionary<String, String>& GetDefaultRequestHeaders()`: Gets the headers which should be sent with each request.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpContent.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Http;

int main() {
    HttpClient client;
    client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupe-Client/4.0");

    // Simple GET
    auto response = client.Get("https://jsonplaceholder.typicode.com/posts/1");
    response->EnsureSuccessStatusCode();

    String jsonBody = response->GetContent()->ReadAsString();
    Console::WriteLine("Response status: {0}", (int)response->GetStatusCode());
    Console::WriteLine("Payload:\n{0}", jsonBody);

    // POST JSON
    auto postContent = SmartPointer<StringContent>::NewShared(
        "{\"title\":\"foo\",\"body\":\"bar\",\"userId\":1}", 
        "application/json");

    auto postResponse = client.Post("https://jsonplaceholder.typicode.com/posts", postContent);
    Console::WriteLine("POST Status: {0}", (int)postResponse->GetStatusCode());

    return 0;
}
```
