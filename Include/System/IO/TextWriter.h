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

                DOTNETDUPE_API virtual void Close();
                DOTNETDUPE_API virtual void Dispose() override;
                DOTNETDUPE_API virtual void Flush();

                DOTNETDUPE_API virtual std::shared_ptr<Text::Encoding> GetEncoding() const = 0;

                DOTNETDUPE_API virtual void Write(bool value);
                DOTNETDUPE_API virtual void Write(char value);
                DOTNETDUPE_API virtual void Write(const char* value);
                DOTNETDUPE_API virtual void Write(const String& value);
                DOTNETDUPE_API virtual void Write(int value);
                DOTNETDUPE_API virtual void Write(long long value);
                DOTNETDUPE_API virtual void Write(float value);
                DOTNETDUPE_API virtual void Write(double value);

                DOTNETDUPE_API virtual void WriteLine();
                DOTNETDUPE_API virtual void WriteLine(bool value);
                DOTNETDUPE_API virtual void WriteLine(char value);
                DOTNETDUPE_API virtual void WriteLine(const char* value);
                DOTNETDUPE_API virtual void WriteLine(const String& value);
                DOTNETDUPE_API virtual void WriteLine(int value);
                DOTNETDUPE_API virtual void WriteLine(long long value);
                DOTNETDUPE_API virtual void WriteLine(float value);
                DOTNETDUPE_API virtual void WriteLine(double value);

            protected:
                TextWriter() = default;
                
                DOTNETDUPE_API static const String& GetNewLine();
            };
        }
    }
}
