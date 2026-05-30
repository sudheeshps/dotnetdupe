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
                DOTNETDUPE_API StringWriter(Text::StringBuilder& sbOutput);
                DOTNETDUPE_API virtual ~StringWriter() = default;

                DOTNETDUPE_API void Close() override;
                DOTNETDUPE_API void Dispose() override;
                DOTNETDUPE_API void Flush() override;
                DOTNETDUPE_API std::shared_ptr<Text::Encoding> GetEncoding() const override;

                DOTNETDUPE_API virtual String ToString() const;

                DOTNETDUPE_API void Write(bool bValue) override;
                DOTNETDUPE_API void Write(char chValue) override;
                DOTNETDUPE_API void Write(const String& sValue) override;
                DOTNETDUPE_API void Write(const char* pValue) override;
                DOTNETDUPE_API void Write(int iValue) override;
                DOTNETDUPE_API void Write(long long llValue) override;
                DOTNETDUPE_API void Write(float fValue) override;
                DOTNETDUPE_API void Write(double dValue) override;

                DOTNETDUPE_API void WriteLine() override;
                DOTNETDUPE_API void WriteLine(bool bValue) override;
                DOTNETDUPE_API void WriteLine(char chValue) override;
                DOTNETDUPE_API void WriteLine(const char* pValue) override;
                DOTNETDUPE_API void WriteLine(const String& sValue) override;
                DOTNETDUPE_API void WriteLine(int iValue) override;
                DOTNETDUPE_API void WriteLine(long long llValue) override;
                DOTNETDUPE_API void WriteLine(float fValue) override;
                DOTNETDUPE_API void WriteLine(double dValue) override;

            private:
                Text::StringBuilder* m_pSb;
                bool m_bIsOpen;
            };
        }
    }
}
