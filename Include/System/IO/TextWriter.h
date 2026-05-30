#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/IO/IDisposable.h"
#include "System/Text/TextEncoding.h"
#include <memory>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class TextWriter : public Object, public IDisposable {
            public:
                virtual ~TextWriter() = default;

                DOTNETDUPE_API virtual void Close() = 0;
                DOTNETDUPE_API virtual void Dispose() override = 0;
                DOTNETDUPE_API virtual void Flush() = 0;

                DOTNETDUPE_API virtual std::shared_ptr<Text::Encoding> GetEncoding() const = 0;

                DOTNETDUPE_API virtual void Write(bool value) = 0;
                DOTNETDUPE_API virtual void Write(char value) = 0;
                DOTNETDUPE_API virtual void Write(const char* value) = 0;
                DOTNETDUPE_API virtual void Write(const String& value) = 0;
                DOTNETDUPE_API virtual void Write(int value) = 0;
                DOTNETDUPE_API virtual void Write(long long value) = 0;
                DOTNETDUPE_API virtual void Write(float value) = 0;
                DOTNETDUPE_API virtual void Write(double value) = 0;

                DOTNETDUPE_API virtual void WriteLine() = 0;
                DOTNETDUPE_API virtual void WriteLine(bool value) = 0;
                DOTNETDUPE_API virtual void WriteLine(char value) = 0;
                DOTNETDUPE_API virtual void WriteLine(const char* value) = 0;
                DOTNETDUPE_API virtual void WriteLine(const String& value) = 0;
                DOTNETDUPE_API virtual void WriteLine(int value) = 0;
                DOTNETDUPE_API virtual void WriteLine(long long value) = 0;
                DOTNETDUPE_API virtual void WriteLine(float value) = 0;
                DOTNETDUPE_API virtual void WriteLine(double value) = 0;

            protected:
                TextWriter() = default;
                
                DOTNETDUPE_API static const String& GetNewLine();
            };
        }
    }
}
