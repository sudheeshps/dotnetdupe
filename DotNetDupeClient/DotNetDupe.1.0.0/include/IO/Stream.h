#pragma once

#include "../Common.h"
#include "../Object.h"
#include "IDisposable.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class Stream : public Object, public IDisposable {
            public:
                virtual ~Stream() = default;

                virtual bool CanRead() const = 0;
                virtual bool CanSeek() const = 0;
                virtual bool CanWrite() const = 0;
                virtual long GetLength() const = 0;
                virtual long GetPosition() const = 0;
                virtual void SetPosition(long value) = 0;

                virtual void Flush() = 0;
                virtual int Read(char* buffer, int offset, int count) = 0;
                virtual long Seek(long offset, int origin) = 0;
                virtual void SetLength(long value) = 0;
                virtual void Write(const char* buffer, int offset, int count) = 0;

                // IDisposable interface
                virtual void Dispose() = 0;
            };
        }
    }
}
