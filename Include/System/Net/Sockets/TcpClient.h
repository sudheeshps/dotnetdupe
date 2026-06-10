#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/NetworkStream.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class TcpClient : public Object {
                public:
                    DOTNETDUPE_API TcpClient();
                    DOTNETDUPE_API explicit TcpClient(AddressFamily family);
                    DOTNETDUPE_API explicit TcpClient(const SmartPointer<Socket>& socket);
                    DOTNETDUPE_API ~TcpClient();

                    TcpClient(const TcpClient&) = delete;
                    TcpClient& operator=(const TcpClient&) = delete;
                    DOTNETDUPE_API TcpClient(TcpClient&& other) noexcept;
                    DOTNETDUPE_API TcpClient& operator=(TcpClient&& other) noexcept;

                    DOTNETDUPE_API void Connect(const String& ip, int port);
                    DOTNETDUPE_API SmartPointer<NetworkStream> GetStream();
                    DOTNETDUPE_API void Close();
                    DOTNETDUPE_API bool Connected() const;

                    DOTNETDUPE_API SmartPointer<Socket> GetClient() const;
                    DOTNETDUPE_API void SetClient(const SmartPointer<Socket>& socket);

                private:
                    SmartPointer<Socket> m_pClientSocket;
                    SmartPointer<NetworkStream> m_pNetworkStream;
                };

            }
        }
    }
}
