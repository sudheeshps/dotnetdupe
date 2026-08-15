#include "pch.h"
#include <mutex>
#include <vector>
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct LoggerFactory::Impl {
                std::vector<DotNetDupe::System::SmartPointer<ILoggerProvider>> pProviders;
                std::mutex mutex;
            };

            LoggerFactory::LoggerFactory()
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {}
                
            LoggerFactory::~LoggerFactory() = default;

            void LoggerFactory::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) {
                std::lock_guard<std::mutex> lock(m_pImpl->mutex);
                m_pImpl->pProviders.push_back(pProvider);
            }

            DotNetDupe::System::SmartPointer<ILogger> LoggerFactory::CreateLogger(const DotNetDupe::System::String& categoryName) {
                std::lock_guard<std::mutex> lock(m_pImpl->mutex);
                auto aggregate = DotNetDupe::System::SmartPointer<AggregateLogger>::NewShared();
                for (auto& pProvider : m_pImpl->pProviders) {
                    auto pLogger = pProvider->CreateLogger(categoryName);
                    if (!pLogger.IsNull()) {
                        aggregate->AddLogger(pLogger);
                    }
                }
                return aggregate;
            }

        }
    }
}
