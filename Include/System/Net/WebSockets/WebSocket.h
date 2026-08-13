#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Threading/Lock.h"
#include "System/Array.h"
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
                    DOTNETDUPE_API ~WebSocket() override;

                    DOTNETDUPE_API WebSocketState GetState() const;
                    DOTNETDUPE_API void SetState(WebSocketState state);

                    DOTNETDUPE_API bool SendAsync(const String& message);
                    DOTNETDUPE_API bool SendBytes(const Array<uint8_t>& data);
                    DOTNETDUPE_API bool ReceiveText(String& outMessage);
                    DOTNETDUPE_API void Close();

                    DOTNETDUPE_API static String ComputeSecWebSocketAccept(const String& secWebSocketKey);

                private:
                    class Impl;
                    SmartPointer<Impl> m_pImpl;
                };

            }
        }
    }
}
