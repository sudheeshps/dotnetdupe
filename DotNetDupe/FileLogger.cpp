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
                /// Bind logger instance to shared file context.
                m_pImpl->pContext = pContext;
            }

            FileLogger::~FileLogger() = default;

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                /// Delegate simple message logging to base implementation.
                LoggerBase::Log(logLevel, message);
            }

            static void PerformFileRollover(const LoggerConfiguration& config, FileLoggerContext* pContext) {
                /// Close current file handle before rotating files on disk.
                pContext->fileStream->close();
                std::string sBaseName = config.FilePath.GetRawString();

                /// Shift existing backup archives (.N to .N+1).
                for (int iIdx = config.Rollover.MaxBackupFiles - 1; iIdx >= 1; --iIdx) {
                    std::string sOldBackup = sBaseName + "." + std::to_string(iIdx);
                    std::string sNewBackup = sBaseName + "." + std::to_string(iIdx + 1);
                    std::remove(sNewBackup.c_str());
                    std::rename(sOldBackup.c_str(), sNewBackup.c_str());
                }

                /// Rename active log file to first backup (.1).
                std::string sFirstBackup = sBaseName + ".1";
                std::remove(sFirstBackup.c_str());
                std::rename(sBaseName.c_str(), sFirstBackup.c_str());

                /// Re-open active log file in append mode.
                pContext->fileStream->open(sBaseName, std::ios::out | std::ios::app);
            }

            static void CheckAndRollover(const LoggerConfiguration& config, FileLoggerContext* pContext, int iLineLength) {
                /// Guard: Skip rollover if disabled or file is not open.
                if (!config.Rollover.EnableRollover || !pContext->fileStream || !pContext->fileStream->is_open()) return;

                /// Trigger rollover if write exceeds maximum configured file size.
                auto currentPos = pContext->fileStream->tellp();
                if (currentPos != std::streampos(-1) && static_cast<long long>(currentPos) + iLineLength >= config.Rollover.MaxFileSizeInBytes) {
                    PerformFileRollover(config, pContext);
                }
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                /// Guard: Check minimum log level and valid file stream.
                if (!IsEnabled(logLevel)) return;
                if (!m_pImpl->pContext || !m_pImpl->pContext->fileStream || !m_pImpl->pContext->fileStream->is_open()) return;

                /// Format log line and acquire mutex lock for thread-safe file writing.
                DotNetDupe::System::String sLine = BuildLogMessage(logLevel, message, properties);
                std::lock_guard<std::mutex> lock(*(m_pImpl->pContext->fileMutex));

                /// Check size and perform rollover if threshold is crossed.
                CheckAndRollover(m_config, m_pImpl->pContext.Get(), sLine.GetLength());

                /// Write line to disk stream.
                if (m_pImpl->pContext->fileStream && m_pImpl->pContext->fileStream->is_open()) {
                    (*(m_pImpl->pContext->fileStream)) << sLine.GetRawString() << std::endl;
                }
            }

        }
    }
}
