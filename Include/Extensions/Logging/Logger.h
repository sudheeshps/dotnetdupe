#pragma once
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            template <typename T>
            class Logger : public ILoggerOf<T> {
            private:
                DotNetDupe::System::SmartPointer<ILogger> m_logger;

            public:
                Logger(const DotNetDupe::System::SmartPointer<ILogger>& logger) : m_logger(logger) {}
                ~Logger() override = default;

                void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override {
                    if (m_logger) {
                        m_logger->Log(logLevel, message);
                    }
                }

                void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override {
                    if (m_logger) {
                        m_logger->Log(logLevel, message, properties);
                    }
                }

                bool IsEnabled(LogLevel logLevel) const override {
                    return m_logger ? m_logger->IsEnabled(logLevel) : false;
                }
            };

        }
    }
}
