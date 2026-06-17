#include "pch.h"
#include "Extensions/Logging/FileLogger.h"
#include <cstdio>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            FileLogger::FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                   std::shared_ptr<std::ofstream> fileStream, std::shared_ptr<std::mutex> fileMutex)
                : LoggerBase(categoryName, config), m_fileStream(fileStream), m_fileMutex(fileMutex) {}

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                LoggerBase::Log(logLevel, message);
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;
                if (!m_fileStream || !m_fileStream->is_open()) return;

                std::string line = BuildLogMessage(logLevel, message, properties);

                // Write thread-safely to file
                std::lock_guard<std::mutex> lock(*m_fileMutex);

                if (m_config.Rollover.EnableRollover && m_fileStream && m_fileStream->is_open()) {
                    auto currentPos = m_fileStream->tellp();
                    if (currentPos != std::streampos(-1)) {
                        long long currentSize = static_cast<long long>(currentPos);
                        if (currentSize + static_cast<long long>(line.length()) >= m_config.Rollover.MaxFileSizeInBytes) {
                            // Close current stream
                            m_fileStream->close();

                            std::string baseName = m_config.FilePath.GetRawString();

                            // Rotate backups: app.log.2 -> app.log.3, etc.
                            for (int i = m_config.Rollover.MaxBackupFiles - 1; i >= 1; --i) {
                                std::string oldBackup = baseName + "." + std::to_string(i);
                                std::string newBackup = baseName + "." + std::to_string(i + 1);
                                std::remove(newBackup.c_str());
                                std::rename(oldBackup.c_str(), newBackup.c_str());
                            }

                            // Rename current file: app.log -> app.log.1
                            std::string firstBackup = baseName + ".1";
                            std::remove(firstBackup.c_str());
                            std::rename(baseName.c_str(), firstBackup.c_str());

                            // Re-open fresh file
                            m_fileStream->open(baseName, std::ios::out | std::ios::app);
                        }
                    }
                }

                if (m_fileStream && m_fileStream->is_open()) {
                    (*m_fileStream) << line << std::endl;
                }
            }

        }
    }
}
