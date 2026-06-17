#include "pch.h"
#include "Extensions/Logging/AggregateLogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            AggregateLogger::AggregateLogger(const std::vector<DotNetDupe::System::SmartPointer<ILogger>>& loggers)
                : m_loggers(loggers) {}

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                for (auto& logger : m_loggers) {
                    logger->Log(logLevel, message);
                }
            }

            void AggregateLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                for (auto& logger : m_loggers) {
                    logger->Log(logLevel, message, properties);
                }
            }

            bool AggregateLogger::IsEnabled(LogLevel logLevel) const {
                for (auto& logger : m_loggers) {
                    if (logger->IsEnabled(logLevel)) return true;
                }
                return false;
            }

        }
    }
}
