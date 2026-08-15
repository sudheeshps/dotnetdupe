#include "pch.h"
#include "System/String.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/ArgumentException.h"
#include <mutex>
#include <cstring>
#include <cstdlib>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    using SockLen = int;
    #ifndef SD_RECEIVE
        #define SD_RECEIVE 0
        #define SD_SEND 1
        #define SD_BOTH 2
    #endif
#else
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/select.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define SD_RECEIVE 0
    #define SD_SEND 1
    #define SD_BOTH 2
    using SockLen = socklen_t;
#endif

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class SocketImpl {
                public:
                    SOCKET hSocket;

                    SocketImpl() : hSocket(INVALID_SOCKET) {}
                    ~SocketImpl() {
                        Cleanup();
                    }

                    void Cleanup() {
                        if (hSocket != INVALID_SOCKET) {
#if defined(_WIN32)
                            closesocket(hSocket);
#else
                            close(hSocket);
#endif
                            hSocket = INVALID_SOCKET;
                        }
                    }
                };

                static int GetLastErrorCode() {
#if defined(_WIN32)
                    return WSAGetLastError();
#else
                    return errno;
#endif
                }

                static void InitializeSockets() {
#if defined(_WIN32)
                    static bool bInitialized = false;
                    static std::mutex mutexInit;
                    std::lock_guard<std::mutex> lock(mutexInit);
                    if (!bInitialized) {
                        WSADATA wsaData;
                        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                            throw SocketException(GetLastErrorCode(), "WSAStartup failed");
                        }
                        bInitialized = true;
                        std::atexit([]() { WSACleanup(); });
                    }
#endif
                }

                static SOCKET CreateNativeSocket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) {
                    int af = (addressFamily == AddressFamily::InterNetworkV6) ? AF_INET6 : AF_INET;
                    int type = (socketType == SocketType::Dgram) ? SOCK_DGRAM : SOCK_STREAM;
                    int proto = (protocolType == ProtocolType::Udp) ? IPPROTO_UDP : IPPROTO_TCP;
#if defined(_WIN32)
                    return socket(af, type, proto);
#else
                    return ::socket(af, type, proto);
#endif
                }

                static void InitSockAddrIn(const String& ip, int port, sockaddr_in& addr) {
                    std::memset(&addr, 0, sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons(static_cast<u_short>(port));
                    if (ip.IsEmpty() || ip == "0.0.0.0") {
                        addr.sin_addr.s_addr = INADDR_ANY;
                    } else {
                        if (inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                            throw ArgumentException("Invalid IP address.");
                        }
                    }
                }

                Socket::Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)
                    : m_pImpl(new SocketImpl()) {
                    InitializeSockets();
                    m_pImpl->hSocket = CreateNativeSocket(addressFamily, socketType, protocolType);
                    if (m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(GetLastErrorCode(), "Failed to create native socket.");
                    }
                }

                Socket::Socket(void* pNativeHandle)
                    : m_pImpl(new SocketImpl()) {
                    InitializeSockets();
#if defined(_WIN32)
                    m_pImpl->hSocket = reinterpret_cast<SOCKET>(pNativeHandle);
#else
                    m_pImpl->hSocket = static_cast<int>(reinterpret_cast<intptr_t>(pNativeHandle));
#endif
                }

                Socket::~Socket() {}

                Socket::Socket(Socket&& other) noexcept : m_pImpl(std::move(other.m_pImpl)) {}

                Socket& Socket::operator=(Socket&& other) noexcept {
                    if (this != &other) m_pImpl = std::move(other.m_pImpl);
                    return *this;
                }

                void Socket::Bind(const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, String("Socket is closed."));
                    sockaddr_in addr;
                    InitSockAddrIn(ip, port, addr);
                    int optval = 1;
#if defined(_WIN32)
                    setsockopt(m_pImpl->hSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval));
#else
                    ::setsockopt(m_pImpl->hSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif
                    if (bind(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), "Failed to bind socket.");
                    }
                }

                void Socket::Listen(int backlog) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, "Socket is closed.");
                    }
                    if (listen(m_pImpl->hSocket, backlog) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), "Failed to listen on socket.");
                    }
                }

                SmartPointer<Socket> Socket::Accept() {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, "Socket is closed.");
                    }

                    sockaddr_in clientAddr;
                    SockLen clientSize = sizeof(clientAddr);
#if defined(_WIN32)
                    SOCKET clientSocket = accept(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
#else
                    int clientSocket = ::accept(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
#endif
                    if (clientSocket == INVALID_SOCKET) {
                        throw SocketException(GetLastErrorCode(), "Failed to accept connection.");
                    }

                    return SmartPointer<Socket>(new Socket(reinterpret_cast<void*>(clientSocket)), true);
                }

                void Socket::Connect(const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, "Socket is closed.");
                    sockaddr_in addr;
                    InitSockAddrIn(ip, port, addr);
                    if (connect(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), "Failed to connect to host.");
                    }
                }

                int Socket::Send(const char* buffer, int offset, int size) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, "Socket is closed.");
                    int bytesSent = send(m_pImpl->hSocket, buffer + offset, size, 0);
                    if (bytesSent == SOCKET_ERROR) throw SocketException(GetLastErrorCode(), "Failed to send data.");
                    return bytesSent;
                }

                int Socket::Receive(char* buffer, int offset, int size) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, "Socket is closed.");
                    int bytesReceived = recv(m_pImpl->hSocket, buffer + offset, size, 0);
                    if (bytesReceived == SOCKET_ERROR) throw SocketException(GetLastErrorCode(), "Failed to receive data.");
                    return bytesReceived;
                }

                int Socket::SendTo(const char* buffer, int offset, int size, const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, "Socket is closed.");
                    sockaddr_in addr;
                    InitSockAddrIn(ip, port, addr);
                    int bytesSent = sendto(m_pImpl->hSocket, buffer + offset, size, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
                    if (bytesSent == SOCKET_ERROR) throw SocketException(GetLastErrorCode(), "Failed to send data to host.");
                    return bytesSent;
                }

                static void FormatSockAddrIp(const sockaddr_in& addr, String& ip, int& port) {
                    char ipBuf[INET_ADDRSTRLEN];
                    if (inet_ntop(AF_INET, &addr.sin_addr, ipBuf, sizeof(ipBuf)) != nullptr) {
                        ip = String(ipBuf);
                    }
                    port = ntohs(addr.sin_port);
                }

                int Socket::ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) throw SocketException(-1, "Socket is closed.");
                    sockaddr_in addr;
                    SockLen addrSize = sizeof(addr);
                    std::memset(&addr, 0, sizeof(addr));
                    int bytesReceived = recvfrom(m_pImpl->hSocket, buffer + offset, size, 0, reinterpret_cast<sockaddr*>(&addr), &addrSize);
                    if (bytesReceived == SOCKET_ERROR) throw SocketException(GetLastErrorCode(), "Failed to receive data from host.");
                    FormatSockAddrIp(addr, ip, port);
                    return bytesReceived;
                }

                void Socket::Close() {
                    if (m_pImpl) m_pImpl->Cleanup();
                }

                void Socket::Shutdown(SocketShutdown how) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) return;
                    int nativeHow = (how == SocketShutdown::Receive) ? SD_RECEIVE : ((how == SocketShutdown::Send) ? SD_SEND : SD_BOTH);
                    shutdown(m_pImpl->hSocket, nativeHow);
                }

                static int SelectSingleSocket(SOCKET hSocket, int microSeconds, SelectMode mode) {
                    fd_set fds;
                    FD_ZERO(&fds);
                    FD_SET(hSocket, &fds);
                    timeval timeout{ microSeconds / 1000000, microSeconds % 1000000 };
                    fd_set* pRead = (mode == SelectMode::SelectRead) ? &fds : nullptr;
                    fd_set* pWrite = (mode == SelectMode::SelectWrite) ? &fds : nullptr;
                    fd_set* pErr = (mode == SelectMode::SelectError) ? &fds : nullptr;
#if defined(_WIN32)
                    return select(0, pRead, pWrite, pErr, &timeout);
#else
                    return select(hSocket + 1, pRead, pWrite, pErr, &timeout);
#endif
                }

                bool Socket::Poll(int microSeconds, SelectMode mode) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) return false;
                    return SelectSingleSocket(m_pImpl->hSocket, microSeconds, mode) > 0;
                }

                static bool IsPeerConnected(SOCKET hSocket) {
                    sockaddr_in peerAddr;
                    SockLen peerLen = sizeof(peerAddr);
                    return (getpeername(hSocket, reinterpret_cast<sockaddr*>(&peerAddr), &peerLen) != SOCKET_ERROR);
                }

                static bool IsSocketActivePeek(SOCKET hSocket) {
                    char buf;
#if defined(_WIN32)
                    int result = recv(hSocket, &buf, 1, MSG_PEEK);
                    if (result == 0) return false;
                    return (result != SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK);
#else
                    int result = ::recv(hSocket, &buf, 1, MSG_PEEK | MSG_DONTWAIT);
                    if (result == 0) return false;
                    return (result != SOCKET_ERROR || (errno == EAGAIN || errno == EWOULDBLOCK));
#endif
                }

                bool Socket::Connected() const {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET || !IsPeerConnected(m_pImpl->hSocket)) return false;
                    int sel = SelectSingleSocket(m_pImpl->hSocket, 0, SelectMode::SelectRead);
                    if (sel == SOCKET_ERROR) return false;
                    if (sel == 0) return true;
                    return IsSocketActivePeek(m_pImpl->hSocket);
                }

                void* Socket::GetNativeHandle() const {
                    return m_pImpl ? reinterpret_cast<void*>(m_pImpl->hSocket) : nullptr;
                }

            }
        }
    }
}
