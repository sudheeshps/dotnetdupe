#pragma once
#include "Common.h"
#include "System/IO/Stream.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class MemoryStream : public Stream {
            public:
                DOTNETDUPE_API MemoryStream();
                DOTNETDUPE_API explicit MemoryStream(const Array<char>& buffer);
                DOTNETDUPE_API MemoryStream(const Array<char>& buffer, bool writable);
                DOTNETDUPE_API ~MemoryStream();

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

                DOTNETDUPE_API Array<char> ToArray() const;

            private:
                struct MemoryStreamImpl;
                MemoryStreamImpl* m_pImpl;
            };
        }
    }
}
