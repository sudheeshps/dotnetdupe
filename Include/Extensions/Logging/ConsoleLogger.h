#pragma once
#include "Common.h"
#include "Extensions/Logging/LoggerBase.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Implementation of ILogger that outputs formatted logs to standard output.
            /// \details Modeled after Microsoft.Extensions.Logging.Console.ConsoleLogger.
            /// Supports ANSI/console color rendering per log level and synchronizes concurrent writes.
            /// \cite Microsoft.Extensions.Logging
            class ConsoleLogger : public LoggerBase {
            public:
                /// \brief Initializes a new instance of ConsoleLogger with category name and configuration.
                /// \param categoryName The logging category name.
                /// \param config LoggerConfiguration controlling minimum level and format.
                DOTNETDUPE_API ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config);

                /// \brief Virtual destructor for polymorphic cleanup.
                DOTNETDUPE_API ~ConsoleLogger() override = default;

                /// \brief Writes a log entry to the console.
                /// \param logLevel Entry severity level.
                /// \param message The message string to log.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;

                /// \brief Writes a structured log entry to the console with properties.
                /// \param logLevel Entry severity level.
                /// \param message The message string to log.
                /// \param properties Dictionary containing structured metadata properties.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
