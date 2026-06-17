#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class ConsoleLogger : public ILogger {
            private:
                DotNetDupe::System::String m_categoryName;
                LoggerConfiguration m_config;

            public:
                DOTNETDUPE_API ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config);
                DOTNETDUPE_API ~ConsoleLogger() override = default;

                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
