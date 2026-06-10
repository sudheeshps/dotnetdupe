#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/TcpClient.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class TcpListener : public Object {
                public:
                    DOTNETDUPE_API TcpListener(int port);
                    DOTNETDUPE_API TcpListener(const String& ip, int port);
                    DOTNETDUPE_API ~TcpListener();

                    TcpListener(const TcpListener&) = delete;
                    TcpListener& operator=(const TcpListener&) = delete;
                    DOTNETDUPE_API TcpListener(TcpListener&& other) noexcept;
                    DOTNETDUPE_API TcpListener& operator=(TcpListener&& other) noexcept;

                    DOTNETDUPE_API void Start();
                    DOTNETDUPE_API void Start(int backlog);
                    DOTNETDUPE_API void Stop();
                    DOTNETDUPE_API SmartPointer<TcpClient> AcceptTcpClient();
                    DOTNETDUPE_API SmartPointer<Socket> AcceptSocket();
                    DOTNETDUPE_API bool Pending();

                private:
                    String m_sIp;
                    int m_nPort;
                    SmartPointer<Socket> m_pListenerSocket;
                    bool m_bActive;
                };

            }
        }
    }
}
