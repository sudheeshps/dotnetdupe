#include "pch.h"
#include "Extensions/Logging/FileLogger.h"
#include <cstdio>
#include "FileLoggerContext.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct FileLogger::Impl {
                DotNetDupe::System::SmartPointer<FileLoggerContext> pContext;
            };

            FileLogger::FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                   const DotNetDupe::System::SmartPointer<FileLoggerContext>& pContext)
                : LoggerBase(categoryName, config), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                m_pImpl->pContext = pContext;
            }

            FileLogger::~FileLogger() = default;

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                LoggerBase::Log(logLevel, message);
            }

            static void PerformFileRollover(const LoggerConfiguration& config, FileLoggerContext* pContext) {
                pContext->fileStream->close();
                std::string baseName = config.FilePath.GetRawString();
                for (int i = config.Rollover.MaxBackupFiles - 1; i >= 1; --i) {
                    std::string oldBackup = baseName + "." + std::to_string(i);
                    std::string newBackup = baseName + "." + std::to_string(i + 1);
                    std::remove(newBackup.c_str());
                    std::rename(oldBackup.c_str(), newBackup.c_str());
                }
                std::string firstBackup = baseName + ".1";
                std::remove(firstBackup.c_str());
                std::rename(baseName.c_str(), firstBackup.c_str());
                pContext->fileStream->open(baseName, std::ios::out | std::ios::app);
            }

            static void CheckAndRollover(const LoggerConfiguration& config, FileLoggerContext* pContext, int lineLength) {
                if (!config.Rollover.EnableRollover || !pContext->fileStream || !pContext->fileStream->is_open()) return;
                auto currentPos = pContext->fileStream->tellp();
                if (currentPos != std::streampos(-1) && static_cast<long long>(currentPos) + lineLength >= config.Rollover.MaxFileSizeInBytes) {
                    PerformFileRollover(config, pContext);
                }
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;
                if (!m_pImpl->pContext || !m_pImpl->pContext->fileStream || !m_pImpl->pContext->fileStream->is_open()) return;

                DotNetDupe::System::String line = BuildLogMessage(logLevel, message, properties);
                std::lock_guard<std::mutex> lock(*(m_pImpl->pContext->fileMutex));
                CheckAndRollover(m_config, m_pImpl->pContext.Get(), line.GetLength());
                if (m_pImpl->pContext->fileStream && m_pImpl->pContext->fileStream->is_open()) {
                    (*(m_pImpl->pContext->fileStream)) << line.GetRawString() << std::endl;
                }
            }

        }
    }
}
