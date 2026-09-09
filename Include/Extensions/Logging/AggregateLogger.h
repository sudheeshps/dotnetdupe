#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Composite ILogger implementation that fans out log messages to multiple underlying loggers.
            /// \details Dispatches Log invocations across all added child loggers, continuing dispatch even
            /// if individual sinks encounter errors. IsEnabled returns true if any child logger is enabled.
            /// \cite Microsoft.Extensions.Logging
            class AggregateLogger : public ILogger {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                /// \brief Initializes an empty AggregateLogger composite.
                DOTNETDUPE_API AggregateLogger();

                /// \brief Destructor releasing aggregated loggers.
                DOTNETDUPE_API ~AggregateLogger() override;

                /// \brief Adds a child ILogger destination to the dispatch list.
                /// \param pLogger SmartPointer to the ILogger to attach.
                DOTNETDUPE_API void AddLogger(const DotNetDupe::System::SmartPointer<ILogger>& pLogger);

                /// \brief Dispatches a plain text message to all attached child loggers.
                /// \param logLevel Entry severity level.
                /// \param message The message text to record.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;

                /// \brief Dispatches a structured message to all attached child loggers.
                /// \param logLevel Entry severity level.
                /// \param message The message text to record.
                /// \param properties Dictionary containing structured metadata properties.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;

                /// \brief Checks if any attached child logger is enabled for the specified level.
                /// \param logLevel Level to be checked.
                /// \return True if at least one child logger returns true for IsEnabled.
                DOTNETDUPE_API bool IsEnabled(LogLevel logLevel) const override;
            };

        }
    }
}
