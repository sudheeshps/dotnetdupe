#include "pch.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/SocketException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                TcpClient::TcpClient() 
                    : m_pClientSocket(new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp), true) {
                }

                TcpClient::TcpClient(AddressFamily family)
                    : m_pClientSocket(new Socket(family, SocketType::Stream, ProtocolType::Tcp), true) {
                }

                TcpClient::TcpClient(const SmartPointer<Socket>& socket)
                    : m_pClientSocket(socket) {
                }

                TcpClient::~TcpClient() {
                    Close();
                }

                TcpClient::TcpClient(TcpClient&& other) noexcept
                    : m_pClientSocket(std::move(other.m_pClientSocket)),
                      m_pNetworkStream(std::move(other.m_pNetworkStream)) {
                }

                TcpClient& TcpClient::operator=(TcpClient&& other) noexcept {
                    if (this != &other) {
                        Close();
                        m_pClientSocket = std::move(other.m_pClientSocket);
                        m_pNetworkStream = std::move(other.m_pNetworkStream);
                    }
                    return *this;
                }

                void TcpClient::Connect(const String& ip, int port) {
                    if (m_pClientSocket.IsNull()) {
                        m_pClientSocket.Attach(new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp), true);
                    }
                    m_pClientSocket->Connect(ip, port);
                }

                SmartPointer<NetworkStream> TcpClient::GetStream() {
                    if (m_pClientSocket.IsNull() || !m_pClientSocket->Connected()) {
                        throw SocketException("TcpClient is not connected.");
                    }
                    if (m_pNetworkStream.IsNull()) {
                        m_pNetworkStream.Attach(new NetworkStream(m_pClientSocket, true), true);
                    }
                    return m_pNetworkStream;
                }

                void TcpClient::Close() {
                    if (!m_pNetworkStream.IsNull()) {
                        m_pNetworkStream->Close();
                        m_pNetworkStream = nullptr;
                    }
                    if (!m_pClientSocket.IsNull()) {
                        m_pClientSocket->Close();
                        m_pClientSocket = nullptr;
                    }
                }

                bool TcpClient::Connected() const {
                    return !m_pClientSocket.IsNull() && m_pClientSocket->Connected();
                }

                SmartPointer<Socket> TcpClient::GetClient() const {
                    return m_pClientSocket;
                }

                void TcpClient::SetClient(const SmartPointer<Socket>& socket) {
                    m_pClientSocket = socket;
                    m_pNetworkStream = nullptr;
                }

            }
        }
    }
}
