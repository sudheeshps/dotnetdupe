#pragma once
#include "Common.h"
#include "Extensions/Logging/LoggerBase.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class ConsoleLogger : public LoggerBase {
            public:
                DOTNETDUPE_API ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config);
                DOTNETDUPE_API ~ConsoleLogger() override = default;

                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
