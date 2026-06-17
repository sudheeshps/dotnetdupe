#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include <string>
#include <sstream>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LoggerBase : public ILogger {
            protected:
                DotNetDupe::System::String m_categoryName;
                LoggerConfiguration m_config;

                std::string FormatLogLine(const std::string& fmt, const std::string& timestamp, const std::string& level, const std::string& category, const std::string& message, const std::string& properties) const;
                std::string GetFormattedTimestamp(const std::string& formatFmt) const;
                const char* LogLevelToString(LogLevel level) const;

                std::string BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
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
