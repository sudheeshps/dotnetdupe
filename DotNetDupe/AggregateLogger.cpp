#include "pch.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct AggregateLogger::Impl {
                std::vector<DotNetDupe::System::SmartPointer<ILogger>> pLoggers;
            };

            AggregateLogger::AggregateLogger()
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {}
                
            AggregateLogger::~AggregateLogger() = default;

            void AggregateLogger::AddLogger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger) {
                m_pImpl->pLoggers.push_back(pLogger);
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                for (auto& pLogger : m_pImpl->pLoggers) {
                    pLogger->Log(logLevel, message);
                }
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                for (auto& pLogger : m_pImpl->pLoggers) {
                    pLogger->Log(logLevel, message, properties);
                }
            }

            bool AggregateLogger::IsEnabled(LogLevel logLevel) const {
                for (auto& pLogger : m_pImpl->pLoggers) {
                    if (pLogger->IsEnabled(logLevel)) return true;
                }
                return false;
            }

        }
    }
}
