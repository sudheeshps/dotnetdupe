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
                            throw SocketException(GetLastErrorCode(), String("WSAStartup failed"));
                        }
                        bInitialized = true;
                        std::atexit([]() { WSACleanup(); });
                    }
#endif
                }

                Socket::Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)
                    : m_pImpl(new SocketImpl()) {
                    InitializeSockets();

                    int af = AF_INET;
                    if (addressFamily == AddressFamily::InterNetworkV6) af = AF_INET6;

                    int type = SOCK_STREAM;
                    if (socketType == SocketType::Dgram) type = SOCK_DGRAM;

                    int proto = IPPROTO_TCP;
                    if (protocolType == ProtocolType::Udp) proto = IPPROTO_UDP;

#if defined(_WIN32)
                    m_pImpl->hSocket = socket(af, type, proto);
#else
                    m_pImpl->hSocket = ::socket(af, type, proto);
#endif
                    if (m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(GetLastErrorCode(), String("Failed to create native socket."));
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

                Socket::~Socket() {
                }

                Socket::Socket(Socket&& other) noexcept
                    : m_pImpl(std::move(other.m_pImpl)) {
                }

                Socket& Socket::operator=(Socket&& other) noexcept {
                    if (this != &other) {
                        m_pImpl = std::move(other.m_pImpl);
                    }
                    return *this;
                }

                void Socket::Bind(const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }

                    sockaddr_in addr;
                    std::memset(&addr, 0, sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons(static_cast<u_short>(port));

                    if (ip.IsEmpty() || ip == "0.0.0.0") {
                        addr.sin_addr.s_addr = INADDR_ANY;
                    } else {
#if defined(_WIN32)
                        if (inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                            throw ArgumentException("Invalid IP address.");
                        }
#else
                        if (::inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                            throw ArgumentException("Invalid IP address.");
                        }
#endif
                    }

                    // Set SO_REUSEADDR to avoid EADDRINUSE on rapid restarts
                    int optval = 1;
#if defined(_WIN32)
                    setsockopt(m_pImpl->hSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval));
#else
                    ::setsockopt(m_pImpl->hSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif

                    if (bind(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to bind socket."));
                    }
                }

                void Socket::Listen(int backlog) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }
                    if (listen(m_pImpl->hSocket, backlog) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to listen on socket."));
                    }
                }

                SmartPointer<Socket> Socket::Accept() {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }

                    sockaddr_in clientAddr;
                    SockLen clientSize = sizeof(clientAddr);
#if defined(_WIN32)
                    SOCKET clientSocket = accept(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
#else
                    int clientSocket = ::accept(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
#endif
                    if (clientSocket == INVALID_SOCKET) {
                        throw SocketException(GetLastErrorCode(), String("Failed to accept connection."));
                    }

                    return SmartPointer<Socket>(new Socket(reinterpret_cast<void*>(clientSocket)), true);
                }

                void Socket::Connect(const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }

                    sockaddr_in addr;
                    std::memset(&addr, 0, sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons(static_cast<u_short>(port));

#if defined(_WIN32)
                    if (inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                        throw ArgumentException("Invalid IP address.");
                    }
#else
                    if (::inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                        throw ArgumentException("Invalid IP address.");
                    }
#endif

                    if (connect(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to connect to host."));
                    }
                }

                int Socket::Send(const char* buffer, int offset, int size) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }
                    int bytesSent = send(m_pImpl->hSocket, buffer + offset, size, 0);
                    if (bytesSent == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to send data."));
                    }
                    return bytesSent;
                }

                int Socket::Receive(char* buffer, int offset, int size) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }
                    int bytesReceived = recv(m_pImpl->hSocket, buffer + offset, size, 0);
                    if (bytesReceived == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to receive data."));
                    }
                    return bytesReceived;
                }

                int Socket::SendTo(const char* buffer, int offset, int size, const String& ip, int port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }

                    sockaddr_in addr;
                    std::memset(&addr, 0, sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons(static_cast<u_short>(port));

#if defined(_WIN32)
                    if (inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                        throw ArgumentException("Invalid IP address.");
                    }
#else
                    if (::inet_pton(AF_INET, ip.GetRawString(), &addr.sin_addr) != 1) {
                        throw ArgumentException("Invalid IP address.");
                    }
#endif

                    int bytesSent = sendto(m_pImpl->hSocket, buffer + offset, size, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
                    if (bytesSent == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to send data to host."));
                    }
                    return bytesSent;
                }

                int Socket::ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        throw SocketException(-1, String("Socket is closed."));
                    }

                    sockaddr_in addr;
                    SockLen addrSize = sizeof(addr);
                    std::memset(&addr, 0, sizeof(addr));

                    int bytesReceived = recvfrom(m_pImpl->hSocket, buffer + offset, size, 0, reinterpret_cast<sockaddr*>(&addr), &addrSize);
                    if (bytesReceived == SOCKET_ERROR) {
                        throw SocketException(GetLastErrorCode(), String("Failed to receive data from host."));
                    }

                    char ipBuf[INET_ADDRSTRLEN];
#if defined(_WIN32)
                    if (inet_ntop(AF_INET, &addr.sin_addr, ipBuf, sizeof(ipBuf)) != nullptr) {
                        ip = String(ipBuf);
                    }
#else
                    if (::inet_ntop(AF_INET, &addr.sin_addr, ipBuf, sizeof(ipBuf)) != nullptr) {
                        ip = String(ipBuf);
                    }
#endif
                    port = ntohs(addr.sin_port);
                    return bytesReceived;
                }

                void Socket::Close() {
                    if (m_pImpl) {
                        m_pImpl->Cleanup();
                    }
                }

                void Socket::Shutdown(SocketShutdown how) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        return;
                    }
                    int nativeHow = SD_BOTH;
                    if (how == SocketShutdown::Receive) {
                        nativeHow = SD_RECEIVE;
                    } else if (how == SocketShutdown::Send) {
                        nativeHow = SD_SEND;
                    }

                    shutdown(m_pImpl->hSocket, nativeHow);
                }

                bool Socket::Poll(int microSeconds, SelectMode mode) {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        return false;
                    }

                    fd_set fds;
                    FD_ZERO(&fds);
                    FD_SET(m_pImpl->hSocket, &fds);

                    timeval timeout;
                    timeout.tv_sec = microSeconds / 1000000;
                    timeout.tv_usec = microSeconds % 1000000;

                    fd_set* readFds = (mode == SelectMode::SelectRead) ? &fds : nullptr;
                    fd_set* writeFds = (mode == SelectMode::SelectWrite) ? &fds : nullptr;
                    fd_set* errorFds = (mode == SelectMode::SelectError) ? &fds : nullptr;

#if defined(_WIN32)
                    int result = select(0, readFds, writeFds, errorFds, &timeout);
#else
                    int result = select(m_pImpl->hSocket + 1, readFds, writeFds, errorFds, &timeout);
#endif
                    return result > 0;
                }

                bool Socket::Connected() const {
                    if (!m_pImpl || m_pImpl->hSocket == INVALID_SOCKET) {
                        return false;
                    }

                    // Check if the socket is actually connected (and not listening, closed, or never connected)
                    sockaddr_in peerAddr;
                    SockLen peerLen = sizeof(peerAddr);
#if defined(_WIN32)
                    if (getpeername(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&peerAddr), &peerLen) == SOCKET_ERROR) {
                        return false;
                    }
#else
                    if (::getpeername(m_pImpl->hSocket, reinterpret_cast<sockaddr*>(&peerAddr), &peerLen) == SOCKET_ERROR) {
                        return false;
                    }
#endif

                    // Check readability with select (timeout 0) to ensure recv won't block
                    fd_set fds;
                    FD_ZERO(&fds);
                    FD_SET(m_pImpl->hSocket, &fds);

                    timeval timeout;
                    timeout.tv_sec = 0;
                    timeout.tv_usec = 0;

#if defined(_WIN32)
                    int selResult = select(0, &fds, nullptr, nullptr, &timeout);
#else
                    int selResult = ::select(m_pImpl->hSocket + 1, &fds, nullptr, nullptr, &timeout);
#endif

                    if (selResult == SOCKET_ERROR) {
                        return false;
                    }

                    if (selResult == 0) {
                        // Not readable, but no error or closure detected -> still connected
                        return true;
                    }

                    char buf;
#if defined(_WIN32)
                    int result = recv(m_pImpl->hSocket, &buf, 1, MSG_PEEK);
                    if (result == 0) return false;
                    if (result == SOCKET_ERROR) {
                        int err = WSAGetLastError();
                        if (err != WSAEWOULDBLOCK) return false;
                    }
#else
                    int result = ::recv(m_pImpl->hSocket, &buf, 1, MSG_PEEK | MSG_DONTWAIT);
                    if (result == 0) return false;
                    if (result == SOCKET_ERROR) {
                        if (errno != EAGAIN && errno != EWOULDBLOCK) return false;
                    }
#endif
                    return true;
                }

                void* Socket::GetNativeHandle() const {
                    return m_pImpl ? reinterpret_cast<void*>(m_pImpl->hSocket) : nullptr;
                }

            }
        }
    }
}
