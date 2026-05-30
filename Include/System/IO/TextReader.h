#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class TextReader : public Object, public IDisposable {
            public:
                virtual ~TextReader() = default;

                DOTNETDUPE_API virtual void Close() = 0;
                DOTNETDUPE_API virtual void Dispose() override = 0;

                DOTNETDUPE_API virtual int Peek() = 0;
                DOTNETDUPE_API virtual int Read() = 0;
                DOTNETDUPE_API virtual int Read(char* pBuffer, int iIndex, int nCount) = 0;
                DOTNETDUPE_API virtual String ReadLine() = 0;
                DOTNETDUPE_API virtual String ReadToEnd() = 0;

            protected:
                TextReader() = default;
            };
        }
    }
}
