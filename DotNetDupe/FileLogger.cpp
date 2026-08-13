#include "pch.h"
#include "Extensions/Logging/FileLogger.h"
#include <cstdio>
#include "FileLoggerContext.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct FileLogger::Impl {
                DotNetDupe::System::SmartPointer<FileLoggerContext> context;
            };

            FileLogger::FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                   const DotNetDupe::System::SmartPointer<FileLoggerContext>& context)
                : LoggerBase(categoryName, config), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                m_pImpl->context = context;
            }

            FileLogger::~FileLogger() = default;

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                LoggerBase::Log(logLevel, message);
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;
                if (!m_pImpl->context || !m_pImpl->context->fileStream || !m_pImpl->context->fileStream->is_open()) return;

                DotNetDupe::System::String line = BuildLogMessage(logLevel, message, properties);

                // Write thread-safely to file
                std::lock_guard<std::mutex> lock(*(m_pImpl->context->fileMutex));

                if (m_config.Rollover.EnableRollover && m_pImpl->context->fileStream && m_pImpl->context->fileStream->is_open()) {
                    auto currentPos = m_pImpl->context->fileStream->tellp();
                    if (currentPos != std::streampos(-1)) {
                        long long currentSize = static_cast<long long>(currentPos);
                        if (currentSize + static_cast<long long>(line.GetLength()) >= m_config.Rollover.MaxFileSizeInBytes) {
                            // Close current stream
                            m_pImpl->context->fileStream->close();

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
                            m_pImpl->context->fileStream->open(baseName, std::ios::out | std::ios::app);
                        }
                    }
                }

                if (m_pImpl->context->fileStream && m_pImpl->context->fileStream->is_open()) {
                    (*(m_pImpl->context->fileStream)) << line.GetRawString() << std::endl;
                }
            }

        }
    }
}
