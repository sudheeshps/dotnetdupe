#include "pch.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            void LoggerFactory::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_pProviders.push_back(pProvider);
            }

            DotNetDupe::System::SmartPointer<ILogger> LoggerFactory::CreateLogger(const DotNetDupe::System::String& categoryName) {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::vector<DotNetDupe::System::SmartPointer<ILogger>> pLoggers;
                for (auto& pProvider : m_pProviders) {
                    auto pLogger = pProvider->CreateLogger(categoryName);
                    if (!pLogger.IsNull()) {
                        pLoggers.push_back(pLogger);
                    }
                }
                return DotNetDupe::System::SmartPointer<AggregateLogger>::NewShared(pLoggers);
            }

        }
    }
}
