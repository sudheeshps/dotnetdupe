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
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Initialize aggregate composite implementation state.
            }
                
            AggregateLogger::~AggregateLogger() = default;

            void AggregateLogger::AddLogger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger) {
                /// Guard: Ignore null logger instances.
                if (pLogger.IsNull()) return;

                /// Append logger destination to internal list.
                m_pImpl->pLoggers.Add(pLogger);
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                /// Fan out log message to all aggregated logger sinks.
                for (int iIdx = 0; iIdx < m_pImpl->pLoggers.GetCount(); ++iIdx) {
                    m_pImpl->pLoggers[iIdx]->Log(logLevel, message);
                }
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                /// Fan out structured message with metadata to all aggregated sinks.
                for (int iIdx = 0; iIdx < m_pImpl->pLoggers.GetCount(); ++iIdx) {
                    m_pImpl->pLoggers[iIdx]->Log(logLevel, message, properties);
                }
            }

            bool AggregateLogger::IsEnabled(LogLevel logLevel) const {
                /// Check if any underlying sink accepts this log level.
                for (int iIdx = 0; iIdx < m_pImpl->pLoggers.GetCount(); ++iIdx) {
                    if (m_pImpl->pLoggers[iIdx]->IsEnabled(logLevel)) return true;
                }

                /// Return false if no sinks accept this level.
                return false;
            }

        }
    }
}
