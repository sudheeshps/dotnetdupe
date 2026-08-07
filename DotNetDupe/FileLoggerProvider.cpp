#include "pch.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "Extensions/Logging/FileLogger.h"
#include "Extensions/Logging/LogManager.h"
#include "System/IO/Path.h"
#include "System/IO/Directory.h"
#include "System/IOException.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            FileLoggerProvider::FileLoggerProvider()
                : FileLoggerProvider(LogManager::GetConfiguration()) {
            }

            static DotNetDupe::System::String ResolveAndPrepareLogPath(const DotNetDupe::System::String& rawFilePath) {
                using namespace DotNetDupe::System::IO;
                DotNetDupe::System::String targetPath = rawFilePath;

                if (targetPath.IsEmpty()) {
                    targetPath = "logs/app.log";
                }

                DotNetDupe::System::String fullPath = Path::GetFullPath(targetPath);
                DotNetDupe::System::String parentDir = Path::GetDirectoryName(fullPath);

                if (!parentDir.IsEmpty() && !Directory::Exists(parentDir)) {
                    Directory::CreateDirectory(parentDir, true);
                }

                return fullPath;
            }

            FileLoggerProvider::FileLoggerProvider(const LoggerConfiguration& config)
                : m_config(config) {
                
                m_config.FilePath = ResolveAndPrepareLogPath(config.FilePath);
                m_fileMutex = std::make_shared<std::mutex>();
                m_fileStream = std::make_shared<std::ofstream>(m_config.FilePath.GetRawString(), std::ios::out | std::ios::app);
                
                if (!m_fileStream->is_open()) {
                    throw DotNetDupe::System::IO::IOException(("Failed to open file for logging: " + m_config.FilePath).GetRawString());
                }
            }

            FileLoggerProvider::FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat, LogLevel minLevel) {
                m_config.FilePath = ResolveAndPrepareLogPath(filePath);
                m_config.IsJsonFormat = isJsonFormat;
                m_config.MinLevel = minLevel;

                m_fileMutex = std::make_shared<std::mutex>();
                m_fileStream = std::make_shared<std::ofstream>(m_config.FilePath.GetRawString(), std::ios::out | std::ios::app);
                
                if (!m_fileStream->is_open()) {
                    throw DotNetDupe::System::IO::IOException(("Failed to open file for logging: " + m_config.FilePath).GetRawString());
                }
            }

            DotNetDupe::System::SmartPointer<ILogger> FileLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                return DotNetDupe::System::SmartPointer<FileLogger>::NewShared(categoryName, m_config, m_fileStream, m_fileMutex);
            }

        }
    }
}
