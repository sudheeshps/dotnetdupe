#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                enum class AddressFamily {
                    Unknown = -1,
                    Unspecified = 0,
                    InterNetwork = 2,
                    InterNetworkV6 = 23
                };

                enum class SocketType {
                    Unknown = -1,
                    Stream = 1,
                    Dgram = 2,
                    Raw = 3
                };

                enum class ProtocolType {
                    Unknown = -1,
                    Tcp = 6,
                    Udp = 17
                };

                enum class SocketShutdown {
                    Receive = 0,
                    Send = 1,
                    Both = 2
                };

                enum class SelectMode {
                    SelectRead = 0,
                    SelectWrite = 1,
                    SelectError = 2
                };

                class SocketImpl;

                class Socket : public Object {
                public:
                    DOTNETDUPE_API Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType);
                    DOTNETDUPE_API ~Socket();

                    Socket(const Socket&) = delete;
                    Socket& operator=(const Socket&) = delete;
                    DOTNETDUPE_API Socket(Socket&& other) noexcept;
                    DOTNETDUPE_API Socket& operator=(Socket&& other) noexcept;

                    DOTNETDUPE_API void Bind(const String& ip, int port);
                    DOTNETDUPE_API void Listen(int backlog);
                    DOTNETDUPE_API SmartPointer<Socket> Accept();
                    DOTNETDUPE_API void Connect(const String& ip, int port);
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size);
                    DOTNETDUPE_API int Receive(char* buffer, int offset, int size);
                    DOTNETDUPE_API int SendTo(const char* buffer, int offset, int size, const String& ip, int port);
                    DOTNETDUPE_API int ReceiveFrom(char* buffer, int offset, int size, String& ip, int& port);
                    DOTNETDUPE_API void Close();
                    DOTNETDUPE_API void Shutdown(SocketShutdown how);
                    DOTNETDUPE_API bool Poll(int microSeconds, SelectMode mode);
                    DOTNETDUPE_API bool Connected() const;

                    DOTNETDUPE_API explicit Socket(void* pNativeHandle);
                    DOTNETDUPE_API void* GetNativeHandle() const;

                private:
                    SmartPointer<SocketImpl> m_pImpl;
                };

            }
        }
    }
}
