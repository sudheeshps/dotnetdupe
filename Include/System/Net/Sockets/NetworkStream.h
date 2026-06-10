#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/Net/Sockets/Socket.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                class NetworkStream : public IO::Stream {
                public:
                    DOTNETDUPE_API explicit NetworkStream(const SmartPointer<Socket>& socket);
                    DOTNETDUPE_API NetworkStream(const SmartPointer<Socket>& socket, bool bOwnsSocket);
                    DOTNETDUPE_API ~NetworkStream();

                    DOTNETDUPE_API bool CanRead() const override;
                    DOTNETDUPE_API bool CanSeek() const override;
                    DOTNETDUPE_API bool CanWrite() const override;
                    DOTNETDUPE_API long GetLength() const override;
                    DOTNETDUPE_API long GetPosition() const override;
                    DOTNETDUPE_API void SetPosition(long value) override;

                    DOTNETDUPE_API void Flush() override;
                    DOTNETDUPE_API int Read(char* buffer, int offset, int count) override;
                    DOTNETDUPE_API long Seek(long offset, int origin) override;
                    DOTNETDUPE_API void SetLength(long value) override;
                    DOTNETDUPE_API void Write(const char* buffer, int offset, int count) override;

                    DOTNETDUPE_API void Dispose() override;
                    DOTNETDUPE_API void Close();

                private:
                    SmartPointer<Socket> m_pSocket;
                    bool m_bOwnsSocket;
                    bool m_bDisposed;
                };

            }
        }
    }
}
