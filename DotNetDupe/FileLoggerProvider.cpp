#include "pch.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "Extensions/Logging/FileLogger.h"
#include "System/IOException.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            FileLoggerProvider::FileLoggerProvider(const LoggerConfiguration& config)
                : m_config(config) {
                
                m_fileMutex = std::make_shared<std::mutex>();
                m_fileStream = std::make_shared<std::ofstream>(config.FilePath.GetRawString(), std::ios::out | std::ios::app);
                
                if (!m_fileStream->is_open()) {
                    throw DotNetDupe::System::IO::IOException(("Failed to open file for logging: " + config.FilePath).GetRawString());
                }
            }

            FileLoggerProvider::FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat, LogLevel minLevel) {
                m_config.FilePath = filePath;
                m_config.IsJsonFormat = isJsonFormat;
                m_config.MinLevel = minLevel;

                m_fileMutex = std::make_shared<std::mutex>();
                m_fileStream = std::make_shared<std::ofstream>(filePath.GetRawString(), std::ios::out | std::ios::app);
                
                if (!m_fileStream->is_open()) {
                    throw DotNetDupe::System::IO::IOException(("Failed to open file for logging: " + filePath).GetRawString());
                }
            }

            DotNetDupe::System::SmartPointer<ILogger> FileLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                return DotNetDupe::System::SmartPointer<FileLogger>::NewShared(categoryName, m_config, m_fileStream, m_fileMutex);
            }

        }
    }
}
