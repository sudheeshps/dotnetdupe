#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Provider that creates FileLogger instances sharing a file handle and rotation context.
            /// \details Modeled after Microsoft.Extensions.Logging file provider extensions.
            /// \cite Microsoft.Extensions.Logging
            class FileLoggerProvider : public ILoggerProvider {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                /// \brief Initializes a new FileLoggerProvider using default configuration.
                DOTNETDUPE_API FileLoggerProvider();

                /// \brief Initializes a new FileLoggerProvider with explicit configuration.
                /// \param config LoggerConfiguration with target file path and rollover options.
                DOTNETDUPE_API FileLoggerProvider(const LoggerConfiguration& config);

                /// \brief Initializes a new FileLoggerProvider with file path, format, and minimum level.
                /// \param filePath Target file path on disk.
                /// \param isJsonFormat True for JSON output, false for plain text.
                /// \param minLevel Minimum LogLevel threshold.
                DOTNETDUPE_API FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat = false, LogLevel minLevel = LogLevel::Information);

                /// \brief Destructor releasing file provider resources.
                DOTNETDUPE_API ~FileLoggerProvider() override;

                /// \brief Gets the configured destination file path.
                /// \return Reference to the file path String.
                DOTNETDUPE_API const DotNetDupe::System::String& GetFilePath() const;

                /// \brief Creates a FileLogger instance for the specified category.
                /// \param categoryName The logging category name.
                /// \return SmartPointer to the instantiated FileLogger.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
