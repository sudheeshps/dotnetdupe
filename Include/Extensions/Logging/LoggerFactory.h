#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include <vector>
#include <mutex>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LoggerFactory : public ILoggerFactory {
            private:
                std::vector<DotNetDupe::System::SmartPointer<ILoggerProvider>> m_pProviders;
                std::mutex m_mutex;

            public:
                DOTNETDUPE_API LoggerFactory() = default;
                DOTNETDUPE_API ~LoggerFactory() override = default;

                DOTNETDUPE_API void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) override;
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
