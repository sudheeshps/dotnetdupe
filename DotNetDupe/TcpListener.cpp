#include "pch.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/SocketException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                TcpListener::TcpListener(int port)
                    : m_sIp("0.0.0.0"), m_nPort(port), m_bActive(false) {
                }

                TcpListener::TcpListener(const String& ip, int port)
                    : m_sIp(ip), m_nPort(port), m_bActive(false) {
                }

                TcpListener::~TcpListener() {
                    /// Stop listener on destruction.
                    Stop();
                }

                TcpListener::TcpListener(TcpListener&& other) noexcept
                    : m_sIp(std::move(other.m_sIp)),
                      m_nPort(other.m_nPort),
                      m_pListenerSocket(std::move(other.m_pListenerSocket)),
                      m_bActive(other.m_bActive) {
                    other.m_bActive = false;
                }

                TcpListener& TcpListener::operator=(TcpListener&& other) noexcept {
                    /// Guard: Check self-assignment.
                    if (this != &other) {
                        Stop();
                        m_sIp = std::move(other.m_sIp);
                        m_nPort = other.m_nPort;
                        m_pListenerSocket = std::move(other.m_pListenerSocket);
                        m_bActive = other.m_bActive;
                        other.m_bActive = false;
                    }
                    return *this;
                }

                void TcpListener::Start() {
                    Start(100);
                }

                void TcpListener::Start(int backlog) {
                    /// Guard: Check already active.
                    if (m_bActive) return;

                    /// Bind and listen on local endpoint.
                    m_pListenerSocket.Attach(new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp), true);
                    m_pListenerSocket->Bind(m_sIp, m_nPort);
                    m_pListenerSocket->Listen(backlog);
                    m_bActive = true;
                }

                void TcpListener::Stop() {
                    /// Deactivate and close listening socket.
                    if (m_bActive) {
                        m_bActive = false;
                        if (!m_pListenerSocket.IsNull()) {
                            m_pListenerSocket->Close();
                        }
                    }
                }

                SmartPointer<TcpClient> TcpListener::AcceptTcpClient() {
                    /// Guard: Verify listener is active.
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        throw SocketException(-1, String("TcpListener is not started."));
                    }

                    /// Accept connection and wrap in TcpClient.
                    SmartPointer<Socket> pListenerSocket = m_pListenerSocket;
                    SmartPointer<Socket> pClientSocket = pListenerSocket->Accept();
                    return SmartPointer<TcpClient>(new TcpClient(pClientSocket));
                }

                SmartPointer<Socket> TcpListener::AcceptSocket() {
                    /// Guard: Verify listener is active.
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        throw SocketException(-1, String("TcpListener is not started."));
                    }

                    /// Accept raw connected socket.
                    SmartPointer<Socket> pListenerSocket = m_pListenerSocket;
                    return pListenerSocket->Accept();
                }

                bool TcpListener::Pending() {
                    /// Guard: Check active status.
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        return false;
                    }

                    /// Poll for readability with zero timeout.
                    return m_pListenerSocket->Poll(0, SelectMode::SelectRead);
                }

            }
        }
    }
}
