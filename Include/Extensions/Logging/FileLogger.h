#pragma once
#include "Common.h"
#include "Extensions/Logging/LoggerBase.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct FileLoggerContext;

            /// \brief Implementation of ILogger that appends formatted logs to a file on disk.
            /// \details Supports file rollover, size-based rotation, timestamp formatting,
            /// and synchronized concurrent file writes across threads.
            /// \cite Microsoft.Extensions.Logging
            class FileLogger : public LoggerBase {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                /// \brief Initializes a new FileLogger instance.
                /// \param categoryName The logging category name.
                /// \param config LoggerConfiguration specifying file path and rotation rules.
                /// \param context Shared file context coordinating file handles and rotation across category loggers.
                DOTNETDUPE_API FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                          const DotNetDupe::System::SmartPointer<FileLoggerContext>& context);

                /// \brief Destructor releasing logger resources.
                DOTNETDUPE_API ~FileLogger() override;

                /// \brief Appends a plain message entry to the log file.
                /// \param logLevel Entry severity level.
                /// \param message The message text to record.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;

                /// \brief Appends a structured message entry to the log file.
                /// \param logLevel Entry severity level.
                /// \param message The message text to record.
                /// \param properties Dictionary containing structured metadata.
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
