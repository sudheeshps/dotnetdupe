#include "pch.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            void LoggerFactory::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& provider) {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_providers.push_back(provider);
            }

            DotNetDupe::System::SmartPointer<ILogger> LoggerFactory::CreateLogger(const DotNetDupe::System::String& categoryName) {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::vector<DotNetDupe::System::SmartPointer<ILogger>> loggers;
                for (auto& provider : m_providers) {
                    auto logger = provider->CreateLogger(categoryName);
                    if (!logger.IsNull()) {
                        loggers.push_back(logger);
                    }
                }
                return DotNetDupe::System::SmartPointer<AggregateLogger>::NewShared(loggers);
            }

        }
    }
}
