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
                DOTNETDUPE_API FileStream(const String& path, int mode);
                DOTNETDUPE_API ~FileStream();

                // Stream overrides
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

                // IDisposable override
                DOTNETDUPE_API void Dispose() override;

            private:
                mutable std::fstream _fileStream;
                String _path;
                int _mode;
                bool _canRead;
                bool _canWrite;
                bool _canSeek;
            };
        }
    }
}
