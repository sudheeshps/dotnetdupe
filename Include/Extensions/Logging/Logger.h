#pragma once
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Delegates logging to a wrapped ILogger instance where category is based on T.
            /// \details Modeled after Microsoft.Extensions.Logging.Logger<T>.
            /// \tparam T The type whose name is used for the logger category name.
            /// \cite Microsoft.Extensions.Logging
            template <typename T>
            class Logger : public ILoggerOf<T> {
            private:
                DotNetDupe::System::SmartPointer<ILogger> m_pLogger;

            public:
                /// \brief Initializes a new instance of Logger<T> wrapping the given ILogger.
                /// \param pLogger SmartPointer to the underlying ILogger implementation.
                Logger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger) : m_pLogger(pLogger) {}

                /// \brief Virtual destructor for polymorphic cleanup.
                ~Logger() override = default;

                /// \brief Writes a log entry at the specified log level.
                /// \param logLevel Entry will be written on this level.
                /// \param message The message string to log.
                void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override {
                    if (m_pLogger) {
                        m_pLogger->Log(logLevel, message);
                    }
                }

                /// \brief Writes a structured log entry with key-value properties.
                /// \param logLevel Entry will be written on this level.
                /// \param message The message string to log.
                /// \param properties Dictionary containing structured log properties.
                void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override {
                    if (m_pLogger) {
                        m_pLogger->Log(logLevel, message, properties);
                    }
                }

                /// \brief Checks if the given logLevel is enabled.
                /// \param logLevel Level to be checked.
                /// \return True if enabled; otherwise, false.
                bool IsEnabled(LogLevel logLevel) const override {
                    return m_pLogger ? m_pLogger->IsEnabled(logLevel) : false;
                }
            };

        }
    }
}
