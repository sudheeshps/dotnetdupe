#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/IOException.h"


namespace DotNetDupe {
    namespace System {
        namespace IO {

            class MockStream : public Stream {
            private:
                struct Impl;
                Impl* m_pImpl;

            public:
                DOTNETDUPE_API MockStream(bool bCanRead = true, bool bCanWrite = true, bool bCanSeek = true);
                DOTNETDUPE_API virtual ~MockStream() override;

                DOTNETDUPE_API void SetThrowOnRead(bool bThrow);
                DOTNETDUPE_API void SetThrowOnWrite(bool bThrow);
                DOTNETDUPE_API void SetCanRead(bool bCanRead);
                DOTNETDUPE_API void SetCanWrite(bool bCanWrite);
                DOTNETDUPE_API void SetCanSeek(bool bCanSeek);
                DOTNETDUPE_API bool IsDisposed() const;

                DOTNETDUPE_API bool CanRead() const override;
                DOTNETDUPE_API bool CanSeek() const override;
                DOTNETDUPE_API bool CanWrite() const override;

                DOTNETDUPE_API long GetLength() const override;
                DOTNETDUPE_API long GetPosition() const override;
                DOTNETDUPE_API void SetPosition(long lValue) override;

                DOTNETDUPE_API void Flush() override;
                DOTNETDUPE_API int Read(char* pBuffer, int iOffset, int iCount) override;
                DOTNETDUPE_API long Seek(long lOffset, int iOrigin) override;
                DOTNETDUPE_API void SetLength(long lValue) override;
                DOTNETDUPE_API void Write(const char* pBuffer, int iOffset, int iCount) override;
                DOTNETDUPE_API void Dispose() override;
            };

        }
    }
}
