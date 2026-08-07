#include "pch.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "Extensions/Logging/LogManager.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            ConsoleLoggerProvider::ConsoleLoggerProvider()
                : ConsoleLoggerProvider(LogManager::GetConfiguration()) {}

            ConsoleLoggerProvider::ConsoleLoggerProvider(const LoggerConfiguration& config)
                : m_config(config) {}

            ConsoleLoggerProvider::ConsoleLoggerProvider(bool isJsonFormat, LogLevel minLevel) {
                m_config.IsJsonFormat = isJsonFormat;
                m_config.MinLevel = minLevel;
            }

            DotNetDupe::System::SmartPointer<ILogger> ConsoleLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                return DotNetDupe::System::SmartPointer<ConsoleLogger>::NewShared(categoryName, m_config);
            }

        }
    }
}
