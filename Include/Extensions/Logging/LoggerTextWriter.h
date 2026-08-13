#pragma once
#include "Common.h"
#include "System/IO/TextWriter.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LoggerTextWriter : public DotNetDupe::System::IO::TextWriter {
            private:
                DotNetDupe::System::SmartPointer<ILogger> m_pLogger;
                LogLevel m_logLevel;

            public:
                DOTNETDUPE_API LoggerTextWriter(
                    const DotNetDupe::System::String& sCategoryName = "Console", 
                    LogLevel logLevel = LogLevel::Information);

                DOTNETDUPE_API LoggerTextWriter(
                    const DotNetDupe::System::SmartPointer<ILogger>& pLogger, 
                    LogLevel logLevel = LogLevel::Information);

                DOTNETDUPE_API virtual ~LoggerTextWriter() override = default;

                DOTNETDUPE_API void Close() override;
                DOTNETDUPE_API void Dispose() override;
                DOTNETDUPE_API void Flush() override;

                DOTNETDUPE_API virtual System::Text::EncodingPtr GetEncoding() const override;

                DOTNETDUPE_API void Write(bool bValue) override;
                DOTNETDUPE_API void Write(char chValue) override;
                DOTNETDUPE_API void Write(const char* pValue) override;
                DOTNETDUPE_API void Write(const DotNetDupe::System::String& sValue) override;
                DOTNETDUPE_API void Write(int iValue) override;
                DOTNETDUPE_API void Write(long long llValue) override;
                DOTNETDUPE_API void Write(float fValue) override;
                DOTNETDUPE_API void Write(double dValue) override;

                DOTNETDUPE_API void WriteLine() override;
                DOTNETDUPE_API void WriteLine(bool bValue) override;
                DOTNETDUPE_API void WriteLine(char chValue) override;
                DOTNETDUPE_API void WriteLine(const char* pValue) override;
                DOTNETDUPE_API void WriteLine(const DotNetDupe::System::String& sValue) override;
                DOTNETDUPE_API void WriteLine(int iValue) override;
                DOTNETDUPE_API void WriteLine(long long llValue) override;
                DOTNETDUPE_API void WriteLine(float fValue) override;
                DOTNETDUPE_API void WriteLine(double dValue) override;
            };

        }
    }
}
