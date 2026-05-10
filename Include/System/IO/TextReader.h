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

                DOTNETDUPE_API virtual void Close();
                DOTNETDUPE_API virtual void Dispose() override;

                DOTNETDUPE_API virtual int Peek();
                DOTNETDUPE_API virtual int Read();
                DOTNETDUPE_API virtual int Read(wchar_t* buffer, int index, int count);
                DOTNETDUPE_API virtual String ReadLine();
                DOTNETDUPE_API virtual String ReadToEnd();

            protected:
                TextReader() = default;
            };
        }
    }
}
