#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include <vector>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class AggregateLogger : public ILogger {
            private:
                std::vector<DotNetDupe::System::SmartPointer<ILogger>> m_pLoggers;

            public:
                DOTNETDUPE_API AggregateLogger(const std::vector<DotNetDupe::System::SmartPointer<ILogger>>& pLoggers);
                DOTNETDUPE_API ~AggregateLogger() override = default;

                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;
            };

        }
    }
}
