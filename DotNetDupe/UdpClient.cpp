#include "pch.h"
#include "System/Net/Sockets/UdpClient.h"
#include "System/Net/Sockets/SocketException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                UdpClient::UdpClient()
                    : m_pClientSocket(new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp), true) {
                }

                UdpClient::UdpClient(int port)
                    : m_pClientSocket(new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp), true) {
                    m_pClientSocket->Bind("0.0.0.0", port);
                }

                UdpClient::UdpClient(const String& ip, int port)
                    : m_pClientSocket(new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp), true) {
                    m_pClientSocket->Bind(ip, port);
                }

                UdpClient::~UdpClient() {
                    /// Close socket on destruction.
                    Close();
                }

                UdpClient::UdpClient(UdpClient&& other) noexcept
                    : m_pClientSocket(std::move(other.m_pClientSocket)) {
                }

                UdpClient& UdpClient::operator=(UdpClient&& other) noexcept {
                    /// Guard: Check self-assignment.
                    if (this != &other) {
                        Close();
                        m_pClientSocket = std::move(other.m_pClientSocket);
                    }
                    return *this;
                }

                void UdpClient::Connect(const String& ip, int port) {
                    /// Lazily instantiate socket if null.
                    if (m_pClientSocket.IsNull()) {
                        m_pClientSocket.Attach(new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp), true);
                    }

                    /// Associate default destination endpoint.
                    m_pClientSocket->Connect(ip, port);
                }

                int UdpClient::Send(const char* buffer, int offset, int size) {
                    /// Guard: Verify open socket.
                    if (m_pClientSocket.IsNull()) {
                        throw SocketException(-1, String("UdpClient is closed."));
                    }

                    /// Send datagram to default remote host.
                    return m_pClientSocket->Send(buffer, offset, size);
                }

                int UdpClient::Send(const char* buffer, int offset, int size, const String& ip, int port) {
                    /// Guard: Verify open socket.
                    if (m_pClientSocket.IsNull()) {
                        throw SocketException(-1, String("UdpClient is closed."));
                    }

                    /// Send datagram to specified endpoint.
                    return m_pClientSocket->SendTo(buffer, offset, size, ip, port);
                }

                int UdpClient::Receive(char* buffer, int offset, int size, String& ip, int& port) {
                    /// Guard: Verify open socket.
                    if (m_pClientSocket.IsNull()) {
                        throw SocketException(-1, String("UdpClient is closed."));
                    }

                    /// Receive datagram and capture sender endpoint.
                    return m_pClientSocket->ReceiveFrom(buffer, offset, size, ip, port);
                }

                void UdpClient::Close() {
                    /// Close underlying socket.
                    if (!m_pClientSocket.IsNull()) {
                        m_pClientSocket->Close();
                        m_pClientSocket = nullptr;
                    }
                }

                SmartPointer<Socket> UdpClient::GetClient() const {
                    return m_pClientSocket;
                }

            }
        }
    }
}
