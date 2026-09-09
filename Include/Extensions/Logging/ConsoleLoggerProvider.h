#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Provider that creates ConsoleLogger instances configured with shared settings.
            /// \details Modeled after Microsoft.Extensions.Logging.Console.ConsoleLoggerProvider.
            /// \cite Microsoft.Extensions.Logging
            class ConsoleLoggerProvider : public ILoggerProvider {
            private:
                LoggerConfiguration m_config;

            public:
                /// \brief Initializes a new ConsoleLoggerProvider using default configuration.
                DOTNETDUPE_API ConsoleLoggerProvider();

                /// \brief Initializes a new ConsoleLoggerProvider using an explicit LoggerConfiguration.
                /// \param config The LoggerConfiguration settings to apply.
                DOTNETDUPE_API ConsoleLoggerProvider(const LoggerConfiguration& config);

                /// \brief Initializes a new ConsoleLoggerProvider with explicit format and minimum level.
                /// \param isJsonFormat True for structured JSON output, false for plain text.
                /// \param minLevel The minimum LogLevel threshold.
                DOTNETDUPE_API ConsoleLoggerProvider(bool isJsonFormat, LogLevel minLevel);

                /// \brief Virtual destructor for polymorphic cleanup.
                DOTNETDUPE_API ~ConsoleLoggerProvider() override = default;

                /// \brief Creates a new ConsoleLogger instance for the given category.
                /// \param categoryName The logging category name.
                /// \return SmartPointer to the newly instantiated ConsoleLogger.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
