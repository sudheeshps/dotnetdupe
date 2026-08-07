#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class ConsoleLoggerProvider : public ILoggerProvider {
            private:
                LoggerConfiguration m_config;

            public:
                DOTNETDUPE_API ConsoleLoggerProvider();
                DOTNETDUPE_API ConsoleLoggerProvider(const LoggerConfiguration& config);
                DOTNETDUPE_API ConsoleLoggerProvider(bool isJsonFormat, LogLevel minLevel);
                DOTNETDUPE_API ~ConsoleLoggerProvider() override = default;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
