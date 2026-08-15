#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/IO/IDisposable.h"
#include "System/Text/TextEncoding.h"


namespace DotNetDupe {
    namespace System {
        namespace IO {
            class TextWriter : public Object, public IDisposable {
            public:
                virtual ~TextWriter() = default;

                DOTNETDUPE_API virtual void Close() = 0;
                DOTNETDUPE_API virtual void Dispose() override = 0;
                DOTNETDUPE_API virtual void Flush() = 0;

                DOTNETDUPE_API virtual Text::EncodingPtr GetEncoding() const = 0;

                DOTNETDUPE_API virtual void Write(bool bValue) = 0;
                DOTNETDUPE_API virtual void Write(char chValue) = 0;
                DOTNETDUPE_API virtual void Write(const char* pValue) = 0;
                DOTNETDUPE_API virtual void Write(const String& sValue) = 0;
                DOTNETDUPE_API virtual void Write(int iValue) = 0;
                DOTNETDUPE_API virtual void Write(long long llValue) = 0;
                DOTNETDUPE_API virtual void Write(float fValue) = 0;
                DOTNETDUPE_API virtual void Write(double dValue) = 0;

                DOTNETDUPE_API virtual void WriteLine() = 0;
                DOTNETDUPE_API virtual void WriteLine(bool bValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(char chValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(const char* pValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(const String& sValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(int iValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(long long llValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(float fValue) = 0;
                DOTNETDUPE_API virtual void WriteLine(double dValue) = 0;

            protected:
                TextWriter() = default;
                
                DOTNETDUPE_API static const String& GetNewLine();
            };
        }
    }
}
