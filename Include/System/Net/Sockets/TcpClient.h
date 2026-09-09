#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/NetworkStream.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \class TcpClient
                /// \brief Provides client connections for TCP network services.
                ///
                /// \details Complies with RFC 793 specifications for TCP client socket communication.
                /// Encapsulates connection lifecycle, stream retrieval, and buffer handling via NetworkStream.
                class TcpClient : public Object {
                public:
                    /// \brief Initializes a new instance of the TcpClient class with AddressFamily::InterNetwork.
                    DOTNETDUPE_API TcpClient();

                    /// \brief Initializes a new instance of the TcpClient class with the specified address family.
                    /// \param family The AddressFamily of the IP network.
                    DOTNETDUPE_API explicit TcpClient(AddressFamily family);

                    /// \brief Initializes a new instance of the TcpClient class wrapping an existing connected Socket.
                    /// \param socket The connected Socket instance.
                    DOTNETDUPE_API explicit TcpClient(const SmartPointer<Socket>& socket);

                    /// \brief Destructor closing connection resources.
                    DOTNETDUPE_API ~TcpClient();

                    TcpClient(const TcpClient&) = delete;
                    TcpClient& operator=(const TcpClient&) = delete;
                    DOTNETDUPE_API TcpClient(TcpClient&& other) noexcept;
                    DOTNETDUPE_API TcpClient& operator=(TcpClient&& other) noexcept;

                    /// \brief Connects the client to a remote TCP host using the specified IP address and port number.
                    /// \param ip The IP address of the host to which you intend to connect.
                    /// \param port The port number to which you intend to connect.
                    DOTNETDUPE_API void Connect(const String& ip, int port);

                    /// \brief Returns the NetworkStream used to send and receive data.
                    /// \return The underlying NetworkStream.
                    /// \throws InvalidOperationException If the TcpClient is not connected.
                    DOTNETDUPE_API SmartPointer<NetworkStream> GetStream();

                    /// \brief Disposes this TcpClient instance and closes the underlying connection.
                    DOTNETDUPE_API void Close();

                    /// \brief Gets a value indicating whether the underlying Socket for a TcpClient is connected to a remote host.
                    /// \return true if the Client socket is connected to a remote resource as of the most recent operation; otherwise, false.
                    DOTNETDUPE_API bool Connected() const;

                    /// \brief Gets the underlying Socket.
                    /// \return The underlying network Socket.
                    DOTNETDUPE_API SmartPointer<Socket> GetClient() const;

                    /// \brief Sets the underlying Socket.
                    /// \param socket The new underlying Socket.
                    DOTNETDUPE_API void SetClient(const SmartPointer<Socket>& socket);

                private:
                    SmartPointer<Socket> m_pClientSocket;
                    SmartPointer<NetworkStream> m_pNetworkStream;
                };

            }
        }
    }
}
