# System.Net.Sockets Namespace

The `System.Net.Sockets` namespace provides a managed implementation of the Windows Sockets (Winsock) / POSIX BSD sockets interface for C++ developers, replicating the .NET Base Class Library (BCL) APIs.

---

## Classes

### class `Socket`

A cross-platform wrapper around native operating system sockets (Winsock on Windows, BSD sockets on POSIX).

#### Namespace
`DotNetDupe::System::Net::Sockets`

#### Constructors
*   `Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)`
    Initializes a new instance of the `Socket` class with the specified address family, socket type, and protocol.
*   `explicit Socket(void* pNativeHandle)`
    Initializes a new instance of the `Socket` class wrapping an existing native socket handle.

#### Methods
*   `void Bind(const String& ip, int port)`
    Associates a `Socket` with a local IP address and port number.
*   `void Listen(int backlog)`
    Places a `Socket` in a listening state to listen for incoming connections.
*   `SmartPointer<Socket> Accept()`
    Accepts an incoming connection request and returns a new `Socket` for communication.
*   `void Connect(const String& ip, int port)`
    Establishes a connection to a remote host.
*   `int Send(const char* buffer, int offset, int size)`
    Sends data to a connected `Socket`.
*   `int Receive(char* buffer, int offset, int size)`
    Receives data from a connected `Socket`.
*   `int SendTo(const char* buffer, int offset, int size, const String& ip, int port)`
    Sends data to the specified endpoint (used for connectionless protocols like UDP).
*   `int ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port)`
    Receives a datagram and stores the source endpoint information.
*   `void Close()`
    Closes the socket connection and releases all associated resources.
*   `void Shutdown(SocketShutdown how)`
    Disables sends and/or receives on a `Socket`.
*   `bool Poll(int microSeconds, SelectMode mode)`
    Polls the status of the socket.
*   `bool Connected() const`
    Gets a value indicating whether a `Socket` is connected to a remote host as of the last I/O operation.
*   `void* GetNativeHandle() const`
    Returns the underlying native socket handle.

---

### class `NetworkStream`

Provides the underlying stream of data for network access. Inherits from `DotNetDupe::System::IO::Stream`.

#### Constructors
*   `explicit NetworkStream(const SmartPointer<Socket>& socket)`
    Initializes a new instance of the `NetworkStream` class for the specified `Socket`.
*   `NetworkStream(const SmartPointer<Socket>& socket, bool bOwnsSocket)`
    Initializes a new instance of the `NetworkStream` class for the specified `Socket`, specifying whether the stream owns the socket.

#### Methods
*   `bool CanRead() const override`
    Gets a value indicating whether the stream supports reading.
*   `bool CanSeek() const override`
    Gets a value indicating whether the stream supports seeking (always returns `false`).
*   `bool CanWrite() const override`
    Gets a value indicating whether the stream supports writing.
*   `long GetLength() const override`
    Throws an `IOException` since `NetworkStream` does not support seeking.
*   `long GetPosition() const override`
    Throws an `IOException` since `NetworkStream` does not support seeking.
*   `void SetPosition(long value) override`
    Throws an `IOException` since `NetworkStream` does not support seeking.
*   `void Flush() override`
    Flushes data from the stream (no-op for `NetworkStream`).
*   `int Read(char* buffer, int offset, int count) override`
    Reads data from the stream into the buffer.
*   `long Seek(long offset, int origin) override`
    Throws an `IOException`.
*   `void SetLength(long value) override`
    Throws an `IOException`.
*   `void Write(const char* buffer, int offset, int count) override`
    Writes data to the stream from the buffer.
*   `void Dispose() override`
    Releases all resources used by the stream and optionally closes the underlying socket.
*   `void Close()`
    Closes the stream.

---

### class `TcpClient`

Provides client connections for TCP network services.

#### Constructors
*   `TcpClient()`
    Initializes a new instance of the `TcpClient` class.
*   `explicit TcpClient(AddressFamily family)`
    Initializes a new instance of the `TcpClient` class with the specified address family.
*   `explicit TcpClient(const SmartPointer<Socket>& socket)`
    Initializes a new instance of the `TcpClient` class wrapping an existing `Socket`.

#### Methods
*   `void Connect(const String& ip, int port)`
    Connects the client to a remote TCP host.
*   `SmartPointer<NetworkStream> GetStream()`
    Returns the `NetworkStream` used to send and receive data.
*   `void Close()`
    Disposes of this `TcpClient` instance and closes the underlying socket connection.
*   `bool Connected() const`
    Gets a value indicating whether the underlying socket is connected.
*   `SmartPointer<Socket> GetClient() const`
    Gets the underlying `Socket`.
*   `void SetClient(const SmartPointer<Socket>& socket)`
    Sets the underlying `Socket`.

---

### class `TcpListener`

Listens for connections from TCP network clients.

#### Constructors
*   `explicit TcpListener(int port)`
    Initializes a new instance of the `TcpListener` class that listens on the specified port.
*   `TcpListener(const String& ip, int port)`
    Initializes a new instance of the `TcpListener` class that listens on the specified IP address and port.

#### Methods
*   `void Start()`
    Starts listening for incoming connection requests.
*   `void Start(int backlog)`
    Starts listening for incoming connection requests with a specified backlog.
*   `void Stop()`
    Closes the listener and stops listening.
*   `SmartPointer<TcpClient> AcceptTcpClient()`
    Accepts a pending connection request and returns a `TcpClient` wrapper.
*   `SmartPointer<Socket> AcceptSocket()`
    Accepts a pending connection request and returns a raw `Socket` wrapper.
*   `bool Pending()`
    Determines if there are pending connection requests.

---

### class `UdpClient`

Provides User Datagram Protocol (UDP) network services.

#### Constructors
*   `UdpClient()`
    Initializes a new instance of the `UdpClient` class.
*   `explicit UdpClient(int port)`
    Initializes a new instance of the `UdpClient` class and binds it to the specified local port.
*   `UdpClient(const String& ip, int port)`
    Initializes a new instance of the `UdpClient` class and binds it to the specified local IP address and port.

#### Methods
*   `void Connect(const String& ip, int port)`
    Establishes a default remote host for sending and receiving datagrams.
*   `int Send(const char* buffer, int offset, int size)`
    Sends a UDP datagram to the connected remote host.
*   `int Send(const char* buffer, int offset, int size, const String& ip, int port)`
    Sends a UDP datagram to the specified remote host and port.
*   `int Receive(char* buffer, int offset, int size, String& ip, int& port)`
    Receives a UDP datagram and returns the sender's endpoint info.
*   `void Close()`
    Closes the UDP connection and releases resources.
*   `SmartPointer<Socket> GetClient() const`
    Gets the underlying `Socket`.

---

### class `SocketException`

The exception that is thrown when a socket error occurs.

#### Constructors
*   `explicit SocketException(const char* message)`
    Initializes a new instance of the `SocketException` class with a specified message.
*   `SocketException(const char* message, int errorCode)`
    Initializes a new instance of the `SocketException` class with a specified message and native OS socket error code.

#### Methods
*   `int GetErrorCode() const`
    Gets the native OS error code associated with the socket error (e.g. `WSAGetLastError()` or `errno`).

---

## Code Example: TCP Client/Listener & UDP Communication

The following is a complete, compile-ready example demonstrating how to use the classes in `System.Net.Sockets` for client-server communication.

```cpp
#include "System/Console.h"
#include "System/String.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/UdpClient.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Net::Sockets;
using namespace DotNetDupe::System::Threading;

void RunServer() {
    try {
        int port = 19100;
        TcpListener listener("127.0.0.1", port);
        listener.Start();
        Console::WriteLine("Server: TcpListener started on 127.0.0.1:19100.");

        // Block until client connects
        SmartPointer<TcpClient> client = listener.AcceptTcpClient();
        Console::WriteLine("Server: Client connected!");

        SmartPointer<NetworkStream> stream = client->GetStream();
        char buffer[128] = {0};
        int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            Console::Write("Server received: '");
            Console::Write(buffer);
            Console::WriteLine("'");
        }

        // Echo back response
        String response = "Hello Client";
        stream->Write(response.GetRawString(), 0, response.GetLength());
        
        client->Close();
        listener.Stop();
        Console::WriteLine("Server: Stopped.");
    } catch (const SocketException& ex) {
        Console::Write("Server Error (SocketException): ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("Server Error: ");
        Console::WriteLine(ex.what());
    }
}

int main() {
    // Start TCP Server in a background thread
    Thread serverThread(RunServer);
    serverThread.Start();

    // Give server thread a moment to start and bind
    Thread::Sleep(200);

    // Run TCP Client
    try {
        TcpClient client;
        Console::WriteLine("Client: Connecting to 127.0.0.1:19100...");
        client.Connect("127.0.0.1", 19100);

        SmartPointer<NetworkStream> stream = client.GetStream();
        String message = "Hello Server";
        Console::WriteLine("Client: Sending request...");
        stream->Write(message.GetRawString(), 0, message.GetLength());

        char buffer[128] = {0};
        int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            Console::Write("Client received response: '");
            Console::Write(buffer);
            Console::WriteLine("'");
        }

        client.Close();
    } catch (const SocketException& ex) {
        Console::Write("Client Error (SocketException): ");
        Console::WriteLine(ex.What());
    }

    serverThread.Join();

    // Demonstrate UDP
    try {
        int udpPort = 19101;
        UdpClient receiver(udpPort);
        UdpClient sender;

        // Run Udp Receive on background thread
        Thread udpReceiveThread([&receiver]() {
            try {
                char buffer[128] = {0};
                String senderIp;
                int senderPort = 0;
                int bytesRead = receiver.Receive(buffer, 0, sizeof(buffer) - 1, senderIp, senderPort);
                if (bytesRead > 0) {
                    Console::Write("UDP Receiver got: '");
                    Console::Write(buffer);
                    Console::Write("' from ");
                    Console::Write(senderIp);
                    Console::Write(":");
                    Console::WriteLine(Convert::ToString(senderPort));
                }
            } catch (...) {}
        });
        udpReceiveThread.Start();

        Thread::Sleep(100);

        // Send datagram
        String datagram = "UdpMessage";
        Console::WriteLine("UDP Sender: Sending datagram...");
        sender.Send(datagram.GetRawString(), 0, datagram.GetLength(), "127.0.0.1", udpPort);

        udpReceiveThread.Join();
        receiver.Close();
        sender.Close();
    } catch (const SocketException& ex) {
        Console::Write("UDP Error: ");
        Console::WriteLine(ex.What());
    }

    return 0;
}
```
