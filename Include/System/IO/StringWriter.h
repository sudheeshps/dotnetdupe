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
                DOTNETDUPE_API void Dispose() override;
                DOTNETDUPE_API void Flush() override;
                DOTNETDUPE_API std::shared_ptr<Text::Encoding> GetEncoding() const override;

                DOTNETDUPE_API virtual String ToString() const;

                DOTNETDUPE_API void Write(bool value) override;
                DOTNETDUPE_API void Write(char value) override;
                DOTNETDUPE_API void Write(const String& value) override;
                DOTNETDUPE_API void Write(const char* value) override;
                DOTNETDUPE_API void Write(int value) override;
                DOTNETDUPE_API void Write(long long value) override;
                DOTNETDUPE_API void Write(float value) override;
                DOTNETDUPE_API void Write(double value) override;

                DOTNETDUPE_API void WriteLine() override;
                DOTNETDUPE_API void WriteLine(bool value) override;
                DOTNETDUPE_API void WriteLine(char value) override;
                DOTNETDUPE_API void WriteLine(const char* value) override;
                DOTNETDUPE_API void WriteLine(const String& value) override;
                DOTNETDUPE_API void WriteLine(int value) override;
                DOTNETDUPE_API void WriteLine(long long value) override;
                DOTNETDUPE_API void WriteLine(float value) override;
                DOTNETDUPE_API void WriteLine(double value) override;

            private:
                Text::StringBuilder* _sb;
                bool _isOpen;
            };
        }
    }
}
