#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LoggerFactory : public ILoggerFactory {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                DOTNETDUPE_API LoggerFactory();
                DOTNETDUPE_API ~LoggerFactory() override;

                DOTNETDUPE_API void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) override;
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
