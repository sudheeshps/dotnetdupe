#include "pch.h"
#include "System/String.h"
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

            void Dns::InitializeSockets() {
#if defined(_WIN32)
                static bool bInitialized = false;
                static std::mutex mutexInit;
                std::lock_guard<std::mutex> lock(mutexInit);
                if (!bInitialized) {
                    WSADATA wsaData;
                    int iErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
                    if (iErr != 0) {
                        throw Sockets::SocketException(iErr, "WSAStartup failed in Dns initialization.");
                    }
                    bInitialized = true;
                }
#endif
            }

            /// Extract IPv4 string representations from linked addrinfo structures.
            static std::vector<String> CollectIpv4Addresses(struct addrinfo* result) {
                std::vector<String> addresses;

                /// Iterate over linked addrinfo nodes.
                for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
                    if (ptr->ai_family == AF_INET) {
                        auto* ipv4 = reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr);
                        char ipAddress[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
                        addresses.push_back(String(ipAddress));
                    }
                }
                return addresses;
            }

            /// Query system DNS resolver via getaddrinfo.
            static struct addrinfo* ResolveHostAddrInfo(const char* host) {
                struct addrinfo hints;
                std::memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                struct addrinfo* result = nullptr;

                /// Invoke native DNS resolution.
                if (getaddrinfo(host, nullptr, &hints, &result) != 0) {
                    throw Sockets::SocketException(-1, "Failed to resolve host.");
                }
                return result;
            }

            Array<String> Dns::GetHostAddresses(const String& hostName) {
                /// Guard: Validate input hostname.
                if (hostName.IsEmpty()) throw ArgumentException("hostName cannot be empty.");

                /// Initialize socket subsystem and resolve address info.
                InitializeSockets();
                struct addrinfo* result = ResolveHostAddrInfo(hostName.GetRawString());
                std::vector<String> addresses = CollectIpv4Addresses(result);
                freeaddrinfo(result);

                /// Verify results and convert to library array.
                if (addresses.empty()) throw Sockets::SocketException(-1, String("No addresses found for host."));
                Array<String> arrAddresses(static_cast<int>(addresses.size()));
                for (size_t i = 0; i < addresses.size(); ++i) arrAddresses[static_cast<int>(i)] = addresses[i];
                return arrAddresses;
            }
        }
    }
}
