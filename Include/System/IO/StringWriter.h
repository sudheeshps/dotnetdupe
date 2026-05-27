#pragma once

#include "Common.h"
#include "System/IO/TextWriter.h"
#include "System/Text/StringBuilder.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class StringWriter : public TextWriter {
            public:
                DOTNETDUPE_API StringWriter();
                DOTNETDUPE_API StringWriter(Text::StringBuilder& sb);
                DOTNETDUPE_API virtual ~StringWriter() = default;

                DOTNETDUPE_API void Close() override;
                DOTNETDUPE_API void Flush() override;
                DOTNETDUPE_API std::shared_ptr<Text::Encoding> GetEncoding() const override;

                DOTNETDUPE_API virtual String ToString() const;

                DOTNETDUPE_API void Write(char value) override;
                DOTNETDUPE_API void Write(const String& value) override;
                DOTNETDUPE_API void Write(const char* value) override;

            private:
                Text::StringBuilder* _sb;
                bool _isOpen;
            };
        }
    }
}
