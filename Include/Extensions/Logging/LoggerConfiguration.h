#pragma once
#include "Common.h"
#include "System/String.h"
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct FileRolloverConfig {
                bool EnableRollover = false;
                long long MaxFileSizeInBytes = 5 * 1024 * 1024; // Default 5 MB
                int MaxBackupFiles = 3; // Keep 3 backup files by default
            };

            // Sample JSON configuration format:
            // {
            //   "MinLevel": "Information",        // Can be a string (case-insensitive) or integer (0-6)
            //   "IsJsonFormat": false,
            //   "PlainTextFormat": "{Timestamp} [{ProcessId}] [{ThreadId}] [{Level}] [{Category}] {Message}",
            //   "TimestampFormat": "%Y-%m-%d %H:%M:%S",
            //   "FilePath": "app.log",
            //   "Rollover": {
            //     "EnableRollover": true,
            //     "MaxFileSizeInBytes": 5242880,
            //     "MaxBackupFiles": 3
            //   }
            // }
            struct LoggerConfiguration {
                LogLevel MinLevel = LogLevel::Information;
                bool IsJsonFormat = false;
                
                // Plain text formatting template: e.g. "{Timestamp} [{ProcessId}:{ThreadId}] [{Level}] [{Category}] {Message}"
                DotNetDupe::System::String PlainTextFormat = "{Timestamp} [{Level}] [{Category}] {Message}";
                
                // Timestamp formatting template using strftime syntax: e.g. "%Y-%m-%d %H:%M:%S"
                DotNetDupe::System::String TimestampFormat = "%Y-%m-%d %H:%M:%S";

                // File path if configured for file logging
                DotNetDupe::System::String FilePath;
                FileRolloverConfig Rollover;

                DOTNETDUPE_API static LoggerConfiguration LoadFromFile(const DotNetDupe::System::String& filePath);
                DOTNETDUPE_API static LoggerConfiguration LoadFromJson(const DotNetDupe::System::String& jsonContent);
            };

            // String parsing utility for LogLevel
            DOTNETDUPE_API LogLevel ParseLogLevel(const DotNetDupe::System::String& str);

        }
    }
}
