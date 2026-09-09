#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \enum AddressFamily
                /// \brief Specifies the addressing scheme that an instance of the Socket class can use.
                enum class AddressFamily {
                    Unknown = -1,
                    Unspecified = 0,
                    InterNetwork = 2,
                    InterNetworkV6 = 23
                };

                /// \enum SocketType
                /// \brief Specifies the type of socket that an instance of the Socket class represents.
                enum class SocketType {
                    Unknown = -1,
                    Stream = 1,
                    Dgram = 2,
                    Raw = 3
                };

                /// \enum ProtocolType
                /// \brief Specifies the protocols that the Socket class supports.
                enum class ProtocolType {
                    Unknown = -1,
                    Tcp = 6,
                    Udp = 17
                };

                /// \enum SocketShutdown
                /// \brief Defines constants that are used by the Socket.Shutdown method.
                enum class SocketShutdown {
                    Receive = 0,
                    Send = 1,
                    Both = 2
                };

                /// \enum SelectMode
                /// \brief Defines the polling modes for the Socket.Poll method.
                enum class SelectMode {
                    SelectRead = 0,
                    SelectWrite = 1,
                    SelectError = 2
                };

                class SocketImpl;

                /// \class Socket
                /// \brief Implements the Berkeley sockets interface.
                ///
                /// \details Provides robust cross-platform network communication compliant with
                /// RFC 793 (Transmission Control Protocol), RFC 768 (User Datagram Protocol),
                /// and POSIX.1-2001 socket APIs. Manages socket descriptors via RAII and encapsulates
                /// Winsock / BSD socket semantics cleanly.
                class Socket : public Object {
                public:
                    /// \brief Initializes a new instance of the Socket class using the specified address family, socket type and protocol.
                    /// \param addressFamily One of the AddressFamily values.
                    /// \param socketType One of the SocketType values.
                    /// \param protocolType One of the ProtocolType values.
                    /// \throws Sockets::SocketException If native socket creation fails.
                    DOTNETDUPE_API Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType);

                    /// \brief Destructor closing encapsulated socket handles.
                    DOTNETDUPE_API ~Socket();

                    Socket(const Socket&) = delete;
                    Socket& operator=(const Socket&) = delete;
                    DOTNETDUPE_API Socket(Socket&& other) noexcept;
                    DOTNETDUPE_API Socket& operator=(Socket&& other) noexcept;

                    /// \brief Associates a Socket with a local endpoint.
                    /// \param ip The local IP address string.
                    /// \param port The local port number.
                    /// \throws Sockets::SocketException If bind operation fails.
                    DOTNETDUPE_API void Bind(const String& ip, int port);

                    /// \brief Places a Socket in a listening state.
                    /// \param backlog The maximum length of the pending connections queue.
                    /// \throws Sockets::SocketException If listen fails.
                    DOTNETDUPE_API void Listen(int backlog);

                    /// \brief Creates a new Socket for a newly created connection.
                    /// \return A Socket for a newly created connection.
                    /// \throws Sockets::SocketException If accept fails.
                    DOTNETDUPE_API SmartPointer<Socket> Accept();

                    /// \brief Establishes a connection to a remote host.
                    /// \param ip The remote IP address string.
                    /// \param port The remote port number.
                    /// \throws Sockets::SocketException If connection fails.
                    DOTNETDUPE_API void Connect(const String& ip, int port);

                    /// \brief Sends data to a connected Socket.
                    /// \param buffer An array of type char that contains the data to be sent.
                    /// \param offset The position in the data buffer at which to begin sending data.
                    /// \param size The number of bytes to send.
                    /// \return The number of bytes sent to the Socket.
                    /// \throws Sockets::SocketException If send fails.
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size);

                    /// \brief Receives data from a bound Socket into a receive buffer.
                    /// \param buffer An array of type char that is the storage location for received data.
                    /// \param offset The location in buffer to store the received data.
                    /// \param size The number of bytes to receive.
                    /// \return The number of bytes received.
                    /// \throws Sockets::SocketException If receive fails.
                    DOTNETDUPE_API int Receive(char* buffer, int offset, int size);

                    /// \brief Sends data to the specified endpoint.
                    /// \param buffer An array of type char that contains the data to be sent.
                    /// \param offset The starting position in buffer.
                    /// \param size The number of bytes to send.
                    /// \param ip The destination IP address.
                    /// \param port The destination port.
                    /// \return The number of bytes sent.
                    /// \throws Sockets::SocketException If sendto fails.
                    DOTNETDUPE_API int SendTo(const char* buffer, int offset, int size, const String& ip, int port);

                    /// \brief Receives a datagram into the data buffer and stores the endpoint.
                    /// \param buffer An array of type char that is the storage location for received data.
                    /// \param offset The location in buffer to store the data.
                    /// \param size The number of bytes to receive.
                    /// \param ip Output parameter receiving the sender's IP address.
                    /// \param port Output parameter receiving the sender's port.
                    /// \return The number of bytes received.
                    /// \throws Sockets::SocketException If recvfrom fails.
                    DOTNETDUPE_API int ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port);

                    /// \brief Closes the Socket connection and releases all associated resources.
                    DOTNETDUPE_API void Close();

                    /// \brief Disables sends and receives on a Socket.
                    /// \param how One of the SocketShutdown values that specifies the operation that will no longer be allowed.
                    DOTNETDUPE_API void Shutdown(SocketShutdown how);

                    /// \brief Determines the status of the Socket.
                    /// \param microSeconds The time to wait for a response, in microseconds.
                    /// \param mode One of the SelectMode values.
                    /// \return The status of the Socket based on the polling mode value passed in the mode parameter.
                    DOTNETDUPE_API bool Poll(int microSeconds, SelectMode mode);

                    /// \brief Gets a value that indicates whether a Socket is connected to a remote host as of the last Send or Receive operation.
                    /// \return true if the Socket was connected as of the most recent operation; otherwise, false.
                    DOTNETDUPE_API bool Connected() const;

                    /// \brief Internal constructor wrapping an existing native socket descriptor.
                    /// \param pNativeHandle Pointer or handle to native socket descriptor.
                    DOTNETDUPE_API explicit Socket(void* pNativeHandle);

                    /// \brief Retrieves the underlying native socket descriptor.
                    /// \return Native socket handle or descriptor pointer.
                    DOTNETDUPE_API void* GetNativeHandle() const;

                private:
                    SmartPointer<SocketImpl> m_pImpl;
                };

            }
        }
    }
}
