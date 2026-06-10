#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class UdpClient : public Object {
                public:
                    DOTNETDUPE_API UdpClient();
                    DOTNETDUPE_API explicit UdpClient(int port);
                    DOTNETDUPE_API UdpClient(const String& ip, int port);
                    DOTNETDUPE_API ~UdpClient();

                    UdpClient(const UdpClient&) = delete;
                    UdpClient& operator=(const UdpClient&) = delete;
                    DOTNETDUPE_API UdpClient(UdpClient&& other) noexcept;
                    DOTNETDUPE_API UdpClient& operator=(UdpClient&& other) noexcept;

                    DOTNETDUPE_API void Connect(const String& ip, int port);
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size);
                    DOTNETDUPE_API int Send(const char* buffer, int offset, int size, const String& ip, int port);
                    DOTNETDUPE_API int Receive(char* buffer, int offset, int size, String& ip, int& port);
                    DOTNETDUPE_API void Close();

                    DOTNETDUPE_API SmartPointer<Socket> GetClient() const;

                private:
                    SmartPointer<Socket> m_pClientSocket;
                };

            }
        }
    }
}
