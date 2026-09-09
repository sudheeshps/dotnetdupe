#include "pch.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "Extensions/Logging/FileLogger.h"
#include "Extensions/Logging/LogManager.h"
#include "System/IO/Path.h"
#include "System/IO/Directory.h"
#include "System/IOException.h"
#include "FileLoggerContext.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            FileLoggerProvider::FileLoggerProvider()
                : FileLoggerProvider(LogManager::GetConfiguration()) {
                /// Initialize with global LogManager configuration.
            }

            static DotNetDupe::System::String ResolveAndPrepareLogPath(const DotNetDupe::System::String& rawFilePath) {
                using namespace DotNetDupe::System::IO;
                /// Fallback to default directory if path is unspecified.
                DotNetDupe::System::String sTargetPath = rawFilePath.IsEmpty() ? DotNetDupe::System::String("logs/app.log") : rawFilePath;

                /// Resolve full normalized path.
                DotNetDupe::System::String sFullPath = Path::GetFullPath(sTargetPath);
                DotNetDupe::System::String sParentDir = Path::GetDirectoryName(sFullPath);

                /// Ensure target parent directory exists on disk.
                if (!sParentDir.IsEmpty() && !Directory::Exists(sParentDir)) {
                    Directory::CreateDirectory(sParentDir, true);
                }

                /// Return normalized full path.
                return sFullPath;
            }

            struct FileLoggerProvider::Impl {
                LoggerConfiguration config;
                DotNetDupe::System::SmartPointer<FileLoggerContext> pContext;
            };

            FileLoggerProvider::FileLoggerProvider(const LoggerConfiguration& config)
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Prepare file path and ensure directory structure.
                m_pImpl->config = config;
                m_pImpl->config.FilePath = ResolveAndPrepareLogPath(config.FilePath);

                /// Initialize shared file context with mutex and file stream.
                m_pImpl->pContext = DotNetDupe::System::SmartPointer<FileLoggerContext>::NewShared();
                m_pImpl->pContext->fileMutex = std::make_shared<std::mutex>();
                m_pImpl->pContext->fileStream = std::make_shared<std::ofstream>(m_pImpl->config.FilePath.GetRawString(), std::ios::out | std::ios::app);
            }

            FileLoggerProvider::FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat, LogLevel minLevel)
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Configure file path, JSON format flag, and minimum severity level.
                m_pImpl->config.FilePath = ResolveAndPrepareLogPath(filePath);
                m_pImpl->config.IsJsonFormat = isJsonFormat;
                m_pImpl->config.MinLevel = minLevel;

                /// Initialize shared file context with mutex and file stream.
                m_pImpl->pContext = DotNetDupe::System::SmartPointer<FileLoggerContext>::NewShared();
                m_pImpl->pContext->fileMutex = std::make_shared<std::mutex>();
                m_pImpl->pContext->fileStream = std::make_shared<std::ofstream>(m_pImpl->config.FilePath.GetRawString(), std::ios::out | std::ios::app);
            }

            FileLoggerProvider::~FileLoggerProvider() {
                /// Safely close the shared file stream on provider destruction.
                if (m_pImpl && m_pImpl->pContext && m_pImpl->pContext->fileMutex && m_pImpl->pContext->fileStream) {
                    std::lock_guard<std::mutex> lock(*(m_pImpl->pContext->fileMutex));
                    if (m_pImpl->pContext->fileStream->is_open()) {
                        m_pImpl->pContext->fileStream->close();
                    }
                }
            }
            
            const DotNetDupe::System::String& FileLoggerProvider::GetFilePath() const {
                /// Return active file path string.
                return m_pImpl->config.FilePath;
            }

            DotNetDupe::System::SmartPointer<ILogger> FileLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                /// Instantiate a new FileLogger sharing the provider's file context.
                auto pLogger = DotNetDupe::System::SmartPointer<FileLogger>::NewShared(categoryName, m_pImpl->config, m_pImpl->pContext);
                return DotNetDupe::System::SmartPointer<ILogger>(pLogger);
            }

        }
    }
}
