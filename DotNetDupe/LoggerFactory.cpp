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
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Initialize logger factory implementation.
            }
                
            LoggerFactory::~LoggerFactory() = default;

            void LoggerFactory::AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) {
                /// Guard: Ignore null provider.
                if (pProvider.IsNull()) return;

                /// Thread-safely register provider.
                std::lock_guard<std::mutex> lock(m_pImpl->mutex);
                m_pImpl->pProviders.push_back(pProvider);
            }

            DotNetDupe::System::SmartPointer<ILogger> LoggerFactory::CreateLogger(const DotNetDupe::System::String& categoryName) {
                /// Synchronize provider access.
                std::lock_guard<std::mutex> lock(m_pImpl->mutex);
                auto pAggregate = DotNetDupe::System::SmartPointer<AggregateLogger>::NewShared();

                /// Query each registered provider to create a category sink.
                for (auto& pProvider : m_pImpl->pProviders) {
                    auto pLogger = pProvider->CreateLogger(categoryName);
                    if (!pLogger.IsNull()) {
                        pAggregate->AddLogger(pLogger);
                    }
                }

                /// Return composite aggregate logger.
                return pAggregate;
            }

        }
    }
}
