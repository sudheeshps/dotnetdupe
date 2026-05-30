#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/String.h"
#include <fstream>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class FileStream : public Stream {
            public:
                DOTNETDUPE_API FileStream(const String& sPath, int iMode);
                DOTNETDUPE_API ~FileStream();

                // Stream overrides
                DOTNETDUPE_API bool CanRead() const override;
                DOTNETDUPE_API bool CanSeek() const override;
                DOTNETDUPE_API bool CanWrite() const override;
                DOTNETDUPE_API long GetLength() const override;
                DOTNETDUPE_API long GetPosition() const override;
                DOTNETDUPE_API void SetPosition(long llValue) override;

                DOTNETDUPE_API void Flush() override;
                DOTNETDUPE_API int Read(char* pBuffer, int iOffset, int nCount) override;
                DOTNETDUPE_API long Seek(long llOffset, int iOrigin) override;
                DOTNETDUPE_API void SetLength(long llValue) override;
                DOTNETDUPE_API void Write(const char* pBuffer, int iOffset, int nCount) override;

                // IDisposable override
                DOTNETDUPE_API void Dispose() override;

            private:
                mutable std::fstream m_fsFileStream;
                String m_sPath;
                int m_iMode;
                bool m_bCanRead;
                bool m_bCanWrite;
                bool m_bCanSeek;
            };
        }
    }
}
