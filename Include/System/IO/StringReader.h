#pragma once

#include "Common.h"
#include "System/IO/TextReader.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class StringReader : public TextReader {
            public:
                DOTNETDUPE_API StringReader(const String& s);
                DOTNETDUPE_API virtual ~StringReader() = default;

                DOTNETDUPE_API void Close() override;
                DOTNETDUPE_API void Dispose() override;
                DOTNETDUPE_API int Peek() override;
                DOTNETDUPE_API int Read() override;
                DOTNETDUPE_API int Read(char* buffer, int index, int count) override;
                DOTNETDUPE_API String ReadLine() override;
                DOTNETDUPE_API String ReadToEnd() override;

            private:
                String _s;
                int _pos;
                int _length;
            };
        }
    }
}
