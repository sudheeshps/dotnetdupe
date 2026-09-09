#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Abstract base class providing common formatting and level filtering for loggers.
            /// \details Modeled after Microsoft.Extensions.Logging provider architectures.
            /// Implements timestamp generation, template string replacement, process/thread metadata extraction,
            /// and standard log line building in both plain text and JSON formats.
            /// \cite Microsoft.Extensions.Logging
            class LoggerBase : public ILogger {
            protected:
                DotNetDupe::System::String m_categoryName;
                LoggerConfiguration m_config;

                /// \brief Formats a single log line according to configuration template.
                DotNetDupe::System::String FormatLogLine(const DotNetDupe::System::String& fmt, const DotNetDupe::System::String& timestamp, const DotNetDupe::System::String& level, const DotNetDupe::System::String& category, const DotNetDupe::System::String& message, const DotNetDupe::System::String& properties, const DotNetDupe::System::String& processId, const DotNetDupe::System::String& threadId) const;

                /// \brief Generates a formatted timestamp string using strftime syntax.
                DotNetDupe::System::String GetFormattedTimestamp(const DotNetDupe::System::String& formatFmt) const;

                /// \brief Converts a LogLevel enumeration to its uppercase string name.
                const char* LogLevelToString(LogLevel level) const;

                /// \brief Builds the final log message string applying plain text or JSON serialization.
                DotNetDupe::System::String BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                            const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) const;

            public:
                /// \brief Initializes a new LoggerBase instance with category name and configuration.
                /// \param categoryName The logging category name.
                /// \param config Logger configuration settings including level and format.
                DOTNETDUPE_API LoggerBase(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config);

                /// \brief Virtual destructor for polymorphic cleanup.
                DOTNETDUPE_API ~LoggerBase() override = default;

                /// \brief Checks whether logging is enabled for the specified log level.
                /// \param logLevel The level to check against configured minimum level.
                /// \return True if logLevel >= configured minimum level; otherwise, false.
                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;

                /// \brief Writes a log entry without extra structured properties.
                /// \param logLevel Entry severity level.
                /// \param message The message string to log.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;

                /// \brief Writes a log entry with extra structured properties.
                /// \param logLevel Entry severity level.
                /// \param message The message string to log.
                /// \param properties Key-value properties associated with this log event.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override = 0;
            };

        }
    }
}
