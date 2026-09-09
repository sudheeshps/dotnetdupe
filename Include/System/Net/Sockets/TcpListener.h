#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/TcpClient.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \class TcpListener
                /// \brief Listens for connections from TCP network clients.
                ///
                /// \details Implements server-side TCP listening services compliant with RFC 793.
                /// Handles port binding, connection queue management, non-blocking polling, and
                /// connection dispatch via AcceptTcpClient and AcceptSocket.
                class TcpListener : public Object {
                public:
                    /// \brief Initializes a new instance of the TcpListener class that listens for incoming connection attempts on the specified local port.
                    /// \param port The port on which to listen for incoming connection attempts.
                    DOTNETDUPE_API TcpListener(int port);

                    /// \brief Initializes a new instance of the TcpListener class that listens on the specified local IP address and port number.
                    /// \param ip An IP address string representing the local network interface.
                    /// \param port The port on which to listen for incoming connection attempts.
                    DOTNETDUPE_API TcpListener(const String& ip, int port);

                    /// \brief Destructor stopping the listener.
                    DOTNETDUPE_API ~TcpListener();

                    TcpListener(const TcpListener&) = delete;
                    TcpListener& operator=(const TcpListener&) = delete;
                    DOTNETDUPE_API TcpListener(TcpListener&& other) noexcept;
                    DOTNETDUPE_API TcpListener& operator=(TcpListener&& other) noexcept;

                    /// \brief Starts listening for incoming connection requests with a default backlog of 100.
                    /// \throws Sockets::SocketException If binding or listening fails.
                    DOTNETDUPE_API void Start();

                    /// \brief Starts listening for incoming connection requests with a specified backlog.
                    /// \param backlog The maximum length of the pending connections queue.
                    /// \throws Sockets::SocketException If binding or listening fails.
                    DOTNETDUPE_API void Start(int backlog);

                    /// \brief Closes the listener.
                    DOTNETDUPE_API void Stop();

                    /// \brief Accepts a pending connection request.
                    /// \return A TcpClient used to send and receive data.
                    /// \throws Sockets::SocketException If the listener has not been started or accept fails.
                    DOTNETDUPE_API SmartPointer<TcpClient> AcceptTcpClient();

                    /// \brief Accepts a pending connection request.
                    /// \return A Socket used to send and receive data.
                    /// \throws Sockets::SocketException If the listener has not been started or accept fails.
                    DOTNETDUPE_API SmartPointer<Socket> AcceptSocket();

                    /// \brief Determines if there are pending connection requests.
                    /// \return true if connections are pending; otherwise, false.
                    DOTNETDUPE_API bool Pending();

                private:
                    String m_sIp;
                    int m_nPort;
                    SmartPointer<Socket> m_pListenerSocket;
                    bool m_bActive;
                };

            }
        }
    }
}
