/// \file Dns.h
/// \brief Provides simple domain name resolution functionality.
///
/// Standard Citation: RFC 1035 Domain Names, RFC 3493 Basic Socket Interface Extensions for IPv6.

#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            /// \class Dns
            /// \brief Provides simple domain name resolution functionality.
            ///
            /// \details Complies with RFC 1035 and RFC 3493 address resolution specifications.
            /// Resolves host names to IP addresses via platform-native getaddrinfo calls.
            class Dns {
            public:
                /// \brief Returns the Internet Protocol (IP) addresses for the specified host.
                /// \param hostName The host name or IP address to resolve.
                /// \return An array of type String that holds the IP addresses for the host.
                /// \throws ArgumentException If hostName is empty.
                /// \throws Sockets::SocketException If host name resolution fails or no addresses are found.
                DOTNETDUPE_API static Array<String> GetHostAddresses(const String& hostName);

            private:
                static void InitializeSockets();
            };
        }
    }
}
