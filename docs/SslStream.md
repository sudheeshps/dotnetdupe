# SslStream

**Namespace:** `DotNetDupe::System::Net::Security`  
**Header:** `#include "System/Net/Security/SslStream.h"`

Provides a stream that uses the Transport Layer Security (TLS/SSL) security protocol to authenticate the server and optionally the client, and to encrypt and sign streams using OpenSSL.

---

## Syntax

```cpp
class SslStream : public IO::Stream;
```

---

## Constructors

### `SslStream(const SmartPointer<IO::Stream>& innerStream)`
Initializes a new instance of the `SslStream` class using the specified `Stream`.

### `SslStream(const SmartPointer<IO::Stream>& innerStream, bool leaveInnerStreamOpen)`
Initializes a new instance of the `SslStream` class with an option to leave the inner stream open when the `SslStream` is disposed.

---

## Authentication Methods

### `void AuthenticateAsClient(const String& targetHost)`
Called by clients to authenticate the server and optionally verify host credentials.
- **Parameters:**
  - `targetHost` (`const String&`): The name of the server that shares this `SslStream`.

### `void AuthenticateAsServer(const SmartPointer<X509Certificate2>& certificate)`
Called by servers to authenticate the server and negotiate TLS cipher suites using the provided X.509 server certificate.

---

## Stream Operations

Implements standard `Stream` operations: `Read`, `Write`, `Flush`, `Seek`, `Dispose`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Security/SslStream.h"
#include "System/SmartPointer.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Net::Security;

int main() {
    TcpClient client;
    client.Connect("www.google.com", 443);

    auto pNetStream = client.GetStream();
    SslStream sslStream(pNetStream);
    sslStream.AuthenticateAsClient("www.google.com");

    const char* request = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    sslStream.Write(request, 0, (int)strlen(request));

    char buffer[512] = { 0 };
    int bytesRead = sslStream.Read(buffer, 0, sizeof(buffer) - 1);
    Console::WriteLine("TLS Response Header:\n{0}", String(buffer));

    return 0;
}
```
