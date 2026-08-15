#include "pch.h"
#include "Extensions/Logging/AggregateLogger.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct AggregateLogger::Impl {
                DotNetDupe::System::Collections::Generic::List<DotNetDupe::System::SmartPointer<ILogger>> pLoggers;
            };

            AggregateLogger::AggregateLogger()
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {}
                
            AggregateLogger::~AggregateLogger() = default;

            void AggregateLogger::AddLogger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger) {
                m_pImpl->pLoggers.Add(pLogger);
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                for (int i = 0; i < m_pImpl->pLoggers.GetCount(); ++i) {
                    m_pImpl->pLoggers[i]->Log(logLevel, message);
                }
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                for (int i = 0; i < m_pImpl->pLoggers.GetCount(); ++i) {
                    m_pImpl->pLoggers[i]->Log(logLevel, message, properties);
                }
            }

            bool AggregateLogger::IsEnabled(LogLevel logLevel) const {
                for (int i = 0; i < m_pImpl->pLoggers.GetCount(); ++i) {
                    if (m_pImpl->pLoggers[i]->IsEnabled(logLevel)) return true;
                }
                return false;
            }

        }
    }
}
