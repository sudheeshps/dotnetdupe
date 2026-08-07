#pragma once
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            template <typename T>
            class Logger : public ILoggerOf<T> {
            private:
                DotNetDupe::System::SmartPointer<ILogger> m_pLogger;

            public:
                Logger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger) : m_pLogger(pLogger) {}
                ~Logger() override = default;

                void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override {
                    if (m_pLogger) {
                        m_pLogger->Log(logLevel, message);
                    }
                }

                void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override {
                    if (m_pLogger) {
                        m_pLogger->Log(logLevel, message, properties);
                    }
                }

                bool IsEnabled(LogLevel logLevel) const override {
                    return m_pLogger ? m_pLogger->IsEnabled(logLevel) : false;
                }
            };

        }
    }
}
