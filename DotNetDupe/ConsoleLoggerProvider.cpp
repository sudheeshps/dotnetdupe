#include "pch.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "Extensions/Logging/LogManager.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            ConsoleLoggerProvider::ConsoleLoggerProvider()
                : ConsoleLoggerProvider(LogManager::GetConfiguration()) {
                /// Initialize with global LogManager configuration.
            }

            ConsoleLoggerProvider::ConsoleLoggerProvider(const LoggerConfiguration& config)
                : m_config(config) {
                /// Initialize with explicit configuration options.
            }

            ConsoleLoggerProvider::ConsoleLoggerProvider(bool isJsonFormat, LogLevel minLevel) {
                /// Configure format flag and minimum severity threshold.
                m_config.IsJsonFormat = isJsonFormat;
                m_config.MinLevel = minLevel;
            }

            DotNetDupe::System::SmartPointer<ILogger> ConsoleLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                /// Instantiate a ConsoleLogger configured for this category.
                auto pLogger = DotNetDupe::System::SmartPointer<ConsoleLogger>::NewShared(categoryName, m_config);

                /// Return the created logger interface.
                return DotNetDupe::System::SmartPointer<ILogger>(pLogger);
            }

        }
    }
}
