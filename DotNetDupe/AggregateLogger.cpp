#include "pch.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            AggregateLogger::AggregateLogger(const std::vector<DotNetDupe::System::SmartPointer<ILogger>>& pLoggers)
                : m_pLoggers(pLoggers) {}

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                for (auto& pLogger : m_pLoggers) {
                    pLogger->Log(logLevel, message);
                }
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                for (auto& pLogger : m_pLoggers) {
                    pLogger->Log(logLevel, message, properties);
                }
            }

            bool AggregateLogger::IsEnabled(LogLevel logLevel) const {
                for (auto& pLogger : m_pLoggers) {
                    if (pLogger->IsEnabled(logLevel)) return true;
                }
                return false;
            }

        }
    }
}
