#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LoggerBase : public ILogger {
            protected:
                DotNetDupe::System::String m_categoryName;
                LoggerConfiguration m_config;

                DotNetDupe::System::String FormatLogLine(const DotNetDupe::System::String& fmt, const DotNetDupe::System::String& timestamp, const DotNetDupe::System::String& level, const DotNetDupe::System::String& category, const DotNetDupe::System::String& message, const DotNetDupe::System::String& properties, const DotNetDupe::System::String& processId, const DotNetDupe::System::String& threadId) const;
                DotNetDupe::System::String GetFormattedTimestamp(const DotNetDupe::System::String& formatFmt) const;
                const char* LogLevelToString(LogLevel level) const;

                DotNetDupe::System::String BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                            const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) const;

            public:
                DOTNETDUPE_API LoggerBase(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config);
                DOTNETDUPE_API ~LoggerBase() override = default;

                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override = 0;
            };

        }
    }
}
