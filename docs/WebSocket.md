# WebSocket &amp; WebSocketException

**Namespace:** `DotNetDupe::System::Net::WebSockets`  
**Header:** `#include "System/Net/WebSockets/WebSocket.h"`, `#include "System/Net/WebSockets/WebSocketException.h"`

Provides the core WebSocket protocol implementation conforming to RFC 6455 for full-duplex communication over TCP streams, along with rich exception-based error reporting.

---

## Enums

### `WebSocketState`
Defines the different states a `WebSocket` instance can be in throughout its lifecycle:
```cpp
enum class WebSocketState {
    None = 0,
    Connecting = 1,
    Open = 2,
    CloseSent = 3,
    CloseReceived = 4,
    Closed = 5,
    Aborted = 6
};
```

### `WebSocketError`
Defines specific error codes associated with `WebSocketException`:
```cpp
enum class WebSocketError {
    Success = 0,
    InvalidMessageType = 1,
    Faulted = 2,
    NativeError = 3,
    NotAWebSocket = 4,
    UnsupportedVersion = 5,
    UnsupportedProtocol = 6,
    HeaderError = 7,
    ConnectionClosedPrematurely = 8,
    InvalidState = 9
};
```

---

## `WebSocket`

### Constructors &amp; Destructor
- `explicit WebSocket(SmartPointer<Sockets::NetworkStream> pStream)`: Initializes a `WebSocket` instance wrapping an underlying `NetworkStream`.
- `virtual ~WebSocket()`: Cleans up resources.

### Member Functions
- `WebSocketState GetState() const`: Returns the current connection state.
- `void SetState(WebSocketState state)`: Updates the current connection state.
- `bool SendAsync(const String& message)`: Sends UTF-8 text framed message over the connection. Throws `WebSocketException` if not in `Open` state or on network failure.
- `bool SendBytes(const Array<uint8_t>& data)`: Sends binary framed data over the connection. Throws `WebSocketException` if not in `Open` state or on network failure.
- `bool ReceiveText(String& outMessage)`: Receives the next text message. Returns `false` upon clean disconnection (close frame `0x08` or clean EOF); throws `WebSocketException` on framing/stream errors.
- `void Close()`: Sends the standard close frame (`0x88`) and transitions the state to `WebSocketState::Closed`.
- `static String ComputeSecWebSocketAccept(const String& secWebSocketKey)`: Computes the `Sec-WebSocket-Accept` header response value according to RFC 6455. Throws `ArgumentException` if key is empty.

---

## `WebSocketException`

Inherits from `DotNetDupe::System::SystemException`.

### Constructors
- `explicit WebSocketException(const String& sMessage)`: Initializes with a custom message and default `WebSocketError::Faulted`.
- `WebSocketException(WebSocketError eError, const String& sMessage)`: Initializes with a specific `WebSocketError` and message.
- `WebSocketException(WebSocketError eError, int iNativeErrorCode, const String& sMessage)`: Initializes with error code, native OS socket error, and message.
- `WebSocketException(WebSocketError eError, const Exception& innerException)`: Initializes with error code and inner exception.
- `WebSocketException(WebSocketError eError, const String& sMessage, const Exception& innerException)`: Initializes with error code, message, and inner exception.

### Member Functions
- `WebSocketError GetWebSocketErrorCode() const`: Gets the `WebSocketError` enum value.
- `int GetErrorCode() const`: Gets the native OS error code, if applicable.
- `const char* What() const`: Gets the descriptive error message.

---

## Usage Example

```cpp
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Net/WebSockets/WebSocketException.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Net::WebSockets;

void RunClient() {
    try {
        TcpClient client;
        client.Connect("127.0.0.1", 8080);
        auto pStream = client.GetStream();

        WebSocket ws(pStream);
        ws.SendAsync("Hello, Server!");

        String response;
        if (ws.ReceiveText(response)) {
            Console::WriteLine(String("Server replied: ") + response);
        }

        ws.Close();
    } catch (const WebSocketException& ex) {
        Console::WriteLine(String("WebSocket error [") + 
            Convert::ToString(static_cast<int>(ex.GetWebSocketErrorCode())) + "]: " + ex.What());
    } catch (const Exception& ex) {
        Console::WriteLine(String("General error: ") + ex.What());
    }
}
```
