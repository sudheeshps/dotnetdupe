#include "pch.h"
#include "System/Net/Dns.h"
#include "System/ArgumentException.h"
#include "System/Net/Sockets/SocketException.h"
#include <vector>
#include <mutex>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <cstring>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Net {

            static void InitializeSockets() {
#if defined(_WIN32)
                static bool bInitialized = false;
                static std::mutex mutexInit;
                std::lock_guard<std::mutex> lock(mutexInit);
                if (!bInitialized) {
                    WSADATA wsaData;
                    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                        // Ignore or throw internally
                    }
                    bInitialized = true;
                }
#endif
            }

            Array<String> Dns::GetHostAddresses(const String& hostName) {
                if (hostName.IsEmpty()) {
                    throw ArgumentException("hostName cannot be empty.");
                }

                InitializeSockets();

                struct addrinfo hints;
                std::memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET; // IPv4 only for compatibility with Socket
                hints.ai_socktype = SOCK_STREAM;

                struct addrinfo* result = nullptr;
                int res = getaddrinfo(hostName.GetRawString(), nullptr, &hints, &result);
                if (res != 0) {
                    throw Sockets::SocketException("Failed to resolve host.", res);
                }

                std::vector<String> addresses;
                for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
                    if (ptr->ai_family == AF_INET) {
                        struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr);
                        char ipAddress[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
                        addresses.push_back(String(ipAddress));
                    }
                }

                freeaddrinfo(result);

                if (addresses.empty()) {
                    throw Sockets::SocketException("No addresses found for host.");
                }

                Array<String> arrAddresses(static_cast<int>(addresses.size()));
                for (size_t i = 0; i < addresses.size(); ++i) {
                    arrAddresses[static_cast<int>(i)] = addresses[i];
                }
                return arrAddresses;
            }
        }
    }
}
