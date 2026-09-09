#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \class UdpClient
                /// \brief Provides User Datagram Protocol (UDP) network services.
                ///
                /// \details Complies with RFC 768 specifications for connectionless datagram transport.
                /// Encapsulates UDP socket setup, binding, unicast/multicast datagram transmission, and receipt.
                class UdpClient : public Object {
                public:
                    /// \brief Initializes a new instance of the UdpClient class.
                    DOTNETDUPE_API UdpClient();

                    /// \brief Initializes a new instance of the UdpClient class and binds it to the specified local port number.
                    /// \param port The local port number from which you intend to communicate.
                    DOTNETDUPE_API explicit UdpClient(int port);

                    /// \brief Initializes a new instance of the UdpClient class and binds it to the specified local IP address and port.
                    /// \param ip The local IP address.
                    /// \param port The local port number.
                    DOTNETDUPE_API UdpClient(const String& ip, int port);

                    /// \brief Destructor closing client socket.
                    DOTNETDUPE_API ~UdpClient();

                    UdpClient(const UdpClient&) = delete;
                    UdpClient& operator=(const UdpClient&) = delete;
                    DOTNETDUPE_API UdpClient(UdpClient&& other) noexcept;
                    DOTNETDUPE_API UdpClient& operator=(UdpClient&& other) noexcept;

                    /// \brief Establishes a default remote host using the specified IP address and port number.
                    /// \param ip The IP address of the remote host.
                    /// \param port The port number on the remote host.
                    DOTNETDUPE_API void Connect(const String& ip, int port);

                    /// \brief Sends a UDP datagram to a remote host (used when Connect has established default host).
                    /// \param buffer An array of type char that contains the data to be sent.
                    /// \param offset The starting position in buffer.
                    /// \param size The number of bytes to send.
                    /// \return The number of bytes sent.
                    /// \throws Sockets::SocketException If the client is closed or transmission fails.
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size);

                    /// \brief Sends a UDP datagram to the specified destination host and port.
                    /// \param buffer An array of type char that contains the data to be sent.
                    /// \param offset The starting position in buffer.
                    /// \param size The number of bytes to send.
                    /// \param ip The destination IP address.
                    /// \param port The destination port.
                    /// \return The number of bytes sent.
                    /// \throws Sockets::SocketException If the client is closed or transmission fails.
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size, const String& ip, int port);

                    /// \brief Returns a UDP datagram that was sent by a remote host.
                    /// \param buffer An array of type char that is the storage location for received data.
                    /// \param offset The position in buffer at which to store the data.
                    /// \param size The number of bytes to receive.
                    /// \param ip Output parameter returning the sender's IP address.
                    /// \param port Output parameter returning the sender's port.
                    /// \return The number of bytes received.
                    /// \throws Sockets::SocketException If the client is closed or receipt fails.
                    DOTNETDUPE_API int Receive(char* buffer, int offset, int size, String& ip, int& port);

                    /// \brief Closes the UDP connection.
                    DOTNETDUPE_API void Close();

                    /// \brief Gets the underlying network Socket.
                    /// \return The underlying network Socket.
                    DOTNETDUPE_API SmartPointer<Socket> GetClient() const;

                private:
                    SmartPointer<Socket> m_pClientSocket;
                };

            }
        }
    }
}
