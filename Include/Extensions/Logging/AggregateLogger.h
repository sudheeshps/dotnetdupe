#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class AggregateLogger : public ILogger {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                DOTNETDUPE_API AggregateLogger();
                DOTNETDUPE_API ~AggregateLogger() override;

                DOTNETDUPE_API void AddLogger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger);

                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;
            };

        }
    }
}
