#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                enum class WebSocketState {
                    None = 0,
                    Connecting = 1,
                    Open = 2,
                    CloseSent = 3,
                    CloseReceived = 4,
                    Closed = 5,
                    Aborted = 6
                };

                class WebSocket : public virtual Object {
                public:
                    DOTNETDUPE_API explicit WebSocket(SmartPointer<Sockets::NetworkStream> pStream);
                    DOTNETDUPE_API ~WebSocket() override = default;

                    DOTNETDUPE_API WebSocketState GetState() const { return m_eState; }
                    DOTNETDUPE_API void SetState(WebSocketState state) { m_eState = state; }

                    DOTNETDUPE_API bool SendAsync(const String& message);
                    DOTNETDUPE_API bool SendBytes(const std::vector<uint8_t>& data);
                    DOTNETDUPE_API bool ReceiveText(String& outMessage);
                    DOTNETDUPE_API void Close();

                    DOTNETDUPE_API static String ComputeSecWebSocketAccept(const String& secWebSocketKey);

                private:
                    SmartPointer<Sockets::NetworkStream> m_pStream;
                    WebSocketState m_eState;
                    Threading::CriticalSection m_csLock;
                };

            }
        }
    }
}
