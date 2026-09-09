#pragma once
#include "Common.h"
#include "System/String.h"
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Configuration settings for file rotation and archival.
            /// \cite Microsoft.Extensions.Logging
            struct FileRolloverConfig {
                /// \brief Indicates whether file rotation is enabled when file size threshold is reached.
                bool EnableRollover = false;
                /// \brief Maximum size in bytes of the primary log file before rotation (default: 5 MB).
                long long MaxFileSizeInBytes = 5 * 1024 * 1024;
                /// \brief Maximum number of rotated backup log files to retain (default: 3).
                int MaxBackupFiles = 3;
            };

            /// \brief Encapsulates logging system configuration options.
            /// \details Modeled after Microsoft.Extensions.Logging configuration patterns.
            /// Supports loading settings from JSON files or raw JSON strings,
            /// setting minimum levels, choosing plain text or JSON output formatting, and setting file paths.
            /// \cite Microsoft.Extensions.Logging
            struct LoggerConfiguration {
                /// \brief Minimum log level required for a message to be recorded.
                LogLevel MinLevel = LogLevel::Information;
                /// \brief Specifies whether to serialize log messages as structured JSON objects.
                bool IsJsonFormat = false;
                
                /// \brief Plain text formatting template (e.g. "{Timestamp} [{Level}] [{Category}] {Message}").
                DotNetDupe::System::String PlainTextFormat = "{Timestamp} [{Level}] [{Category}] {Message}";
                
                /// \brief Timestamp formatting template using strftime syntax (e.g. "%Y-%m-%d %H:%M:%S").
                DotNetDupe::System::String TimestampFormat = "%Y-%m-%d %H:%M:%S";

                /// \brief Destination file path when file logging is configured.
                DotNetDupe::System::String FilePath;
                /// \brief File rotation settings.
                FileRolloverConfig Rollover;

                /// \brief Loads logging configuration options from a JSON file.
                /// \param filePath Path to the JSON configuration file on disk.
                /// \return Parsed LoggerConfiguration object.
                DOTNETDUPE_API static LoggerConfiguration LoadFromFile(const DotNetDupe::System::String& filePath);

                /// \brief Loads logging configuration options from a raw JSON string.
                /// \param jsonContent String containing valid JSON configuration.
                /// \return Parsed LoggerConfiguration object.
                DOTNETDUPE_API static LoggerConfiguration LoadFromJson(const DotNetDupe::System::String& jsonContent);
            };

            /// \brief Parses a case-insensitive string into a corresponding LogLevel enumeration value.
            /// \param str String name of the log level (e.g., "Information", "Debug", "Error").
            /// \return The parsed LogLevel enum value, or LogLevel::Information on unrecognized input.
            DOTNETDUPE_API LogLevel ParseLogLevel(const DotNetDupe::System::String& str);

        }
    }
}
