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
                    if (m_bActive) return;

                    m_pListenerSocket.Attach(new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp), true);
                    m_pListenerSocket->Bind(m_sIp, m_nPort);
                    m_pListenerSocket->Listen(backlog);
                    m_bActive = true;
                }

                void TcpListener::Stop() {
                    if (m_bActive) {
                        m_bActive = false;
                        if (!m_pListenerSocket.IsNull()) {
                            m_pListenerSocket->Close();
                            m_pListenerSocket = nullptr;
                        }
                    }
                }

                SmartPointer<TcpClient> TcpListener::AcceptTcpClient() {
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        throw SocketException("TcpListener is not started.");
                    }
                    SmartPointer<Socket> clientSocket = m_pListenerSocket->Accept();
                    return SmartPointer<TcpClient>(new TcpClient(clientSocket));
                }

                SmartPointer<Socket> TcpListener::AcceptSocket() {
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        throw SocketException("TcpListener is not started.");
                    }
                    return m_pListenerSocket->Accept();
                }

                bool TcpListener::Pending() {
                    if (!m_bActive || m_pListenerSocket.IsNull()) {
                        return false;
                    }
                    return m_pListenerSocket->Poll(0, SelectMode::SelectRead);
                }

            }
        }
    }
}
