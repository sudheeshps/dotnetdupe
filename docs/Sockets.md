# Sockets, NetworkStream, TcpClient, TcpListener &amp; UdpClient

**Namespace:** `DotNetDupe::System::Net::Sockets`  
**Header:** `#include "System/Net/Sockets/Socket.h"`, `#include "System/Net/Sockets/NetworkStream.h"`, `#include "System/Net/Sockets/TcpClient.h"`, `#include "System/Net/Sockets/TcpListener.h"`, `#include "System/Net/Sockets/UdpClient.h"`

Provides a cross-platform implementation of Berkeley sockets (WinSock on Windows, POSIX BSD sockets on Linux), client/listener TCP connections, UDP transmission, and socket-backed network streams.

---

## Enums

```cpp
enum class AddressFamily { Unknown = -1, Unspecified = 0, InterNetwork = 2, InterNetworkV6 = 23 };
enum class SocketType    { Unknown = -1, Stream = 1, Dgram = 2, Raw = 3 };
enum class ProtocolType  { Unknown = -1, Tcp = 6, Udp = 17 };
enum class SocketShutdown{ Receive = 0, Send = 1, Both = 2 };
enum class SelectMode    { SelectRead = 0, SelectWrite = 1, SelectError = 2 };
```

---

## `Socket`

### Constructors
- `Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)`: Initializes a new socket.
- `explicit Socket(void* pNativeHandle)`: Wraps an existing native OS socket handle.

### Member Functions
- `void Bind(const String& ip, int port)`: Associates a `Socket` with a local endpoint.
- `void Listen(int backlog)`: Places a `Socket` in a listening state.
- `SmartPointer<Socket> Accept()`: Creates a new `Socket` for a newly created connection.
- `void Connect(const String& ip, int port)`: Establishes a connection to a remote host.
- `int Send(const char* buffer, int offset, int size)`: Sends data to a connected `Socket`.
- `int Receive(char* buffer, int offset, int size)`: Receives data from a bound `Socket`.
- `int SendTo(const char* buffer, int offset, int size, const String& ip, int port)`: Sends datagram to a specific endpoint.
- `int ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port)`: Receives datagram and stores source endpoint.
- `void Shutdown(SocketShutdown how)`: Disables sends and/or receives on a `Socket`.
- `void Close()`: Closes the socket connection and releases all resources.
- `bool Poll(int microSeconds, SelectMode mode)`: Determines the status of the `Socket`.
- `bool Connected() const`: Gets a value indicating whether a `Socket` is connected.
- `void* GetNativeHandle() const`: Returns the internal native OS socket descriptor.

---

## `NetworkStream`

Provides the underlying stream of data for network access (inherits `System::IO::Stream`).

### Constructors
- `explicit NetworkStream(const SmartPointer<Socket>& socket)`
- `NetworkStream(const SmartPointer<Socket>& socket, bool bOwnsSocket)`

### Member Functions
- Standard `Stream` overrides: `Read`, `Write`, `Flush`, `Close`, `Dispose`.

---

## `TcpClient`

Provides client connections for TCP network services.

### Constructors
- `TcpClient()`
- `explicit TcpClient(AddressFamily family)`
- `explicit TcpClient(const SmartPointer<Socket>& socket)`

### Member Functions
- `void Connect(const String& ip, int port)`: Connects the client to a remote TCP host.
- `SmartPointer<NetworkStream> GetStream()`: Returns the `NetworkStream` used to send and receive data.
- `void Close()`: Disposes this `TcpClient` instance.
- `bool Connected() const`: Gets a value indicating whether the underlying socket is connected.
- `SmartPointer<Socket> GetClient() const`: Gets the underlying `Socket`.

---

## `TcpListener`

Listens for connections from TCP network clients.

### Constructors
- `TcpListener(int port)`: Initializes a new instance listening on all local network addresses.
- `TcpListener(const String& ip, int port)`: Initializes a new instance listening on a specific local IP.

### Member Functions
- `void Start(int backlog = 100)`: Starts listening for incoming connection requests.
- `void Stop()`: Closes the listener.
- `SmartPointer<TcpClient> AcceptTcpClient()`: Accepts a pending connection request as a `TcpClient`.
- `SmartPointer<Socket> AcceptSocket()`: Accepts a pending connection request as a `Socket`.
- `bool Pending()`: Determines if there are pending connection requests.

---

## `UdpClient`

Provides User Datagram Protocol (UDP) network services.

### Constructors
- `UdpClient()`
- `explicit UdpClient(int port)`
- `UdpClient(const String& ip, int port)`

### Member Functions
- `void Connect(const String& ip, int port)`: Establishes a default remote host.
- `int Send(const char* buffer, int offset, int size)`: Sends a UDP datagram to the connected host.
- `int Send(const char* buffer, int offset, int size, const String& ip, int port)`: Sends a UDP datagram to the specified destination.
- `int Receive(char* buffer, int offset, int size, String& ip, int& port)`: Returns a UDP datagram and updates source address/port.
- `void Close()`: Closes the UDP connection.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

int main() {
    int port = 9080;

    // Server
    ThreadPool::QueueUserWorkItem([port](Object* state) {
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        auto client = listener.AcceptTcpClient();
        auto stream = client->GetStream();

        char buf[64] = { 0 };
        int bytes = stream->Read(buf, 0, sizeof(buf) - 1);
        Console::WriteLine("Server received: {0}", String(buf));

        const char* reply = "PONG";
        stream->Write(reply, 0, 4);

        listener.Stop();
    });

    Thread::Sleep(50); // Allow server to bind

    // Client
    TcpClient client;
    client.Connect("127.0.0.1", port);
    auto stream = client.GetStream();

    const char* ping = "PING";
    stream->Write(ping, 0, 4);

    char replyBuf[64] = { 0 };
    stream->Read(replyBuf, 0, sizeof(replyBuf) - 1);
    Console::WriteLine("Client received reply: {0}", String(replyBuf));

    return 0;
}
```
