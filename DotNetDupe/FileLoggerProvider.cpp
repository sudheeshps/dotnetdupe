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

            struct FileLoggerProvider::Impl {
                LoggerConfiguration config;
                DotNetDupe::System::SmartPointer<FileLoggerContext> context;
            };

            FileLoggerProvider::FileLoggerProvider(const LoggerConfiguration& config)
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                m_pImpl->config = config;
                m_pImpl->config.FilePath = ResolveAndPrepareLogPath(config.FilePath);
                m_pImpl->context = DotNetDupe::System::SmartPointer<FileLoggerContext>::NewShared();
                m_pImpl->context->fileMutex = std::make_shared<std::mutex>();
                m_pImpl->context->fileStream = std::make_shared<std::ofstream>(m_pImpl->config.FilePath.GetRawString(), std::ios::out | std::ios::app);
            }

            FileLoggerProvider::FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat, LogLevel minLevel)
                : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                m_pImpl->config.FilePath = ResolveAndPrepareLogPath(filePath);
                m_pImpl->config.IsJsonFormat = isJsonFormat;
                m_pImpl->config.MinLevel = minLevel;
                m_pImpl->context = DotNetDupe::System::SmartPointer<FileLoggerContext>::NewShared();
                m_pImpl->context->fileMutex = std::make_shared<std::mutex>();
                m_pImpl->context->fileStream = std::make_shared<std::ofstream>(m_pImpl->config.FilePath.GetRawString(), std::ios::out | std::ios::app);
            }

            FileLoggerProvider::~FileLoggerProvider() {
                if (m_pImpl && m_pImpl->context && m_pImpl->context->fileMutex && m_pImpl->context->fileStream) {
                    std::lock_guard<std::mutex> lock(*(m_pImpl->context->fileMutex));
                    if (m_pImpl->context->fileStream->is_open()) {
                        m_pImpl->context->fileStream->close();
                    }
                }
            }
            
            const DotNetDupe::System::String& FileLoggerProvider::GetFilePath() const {
                return m_pImpl->config.FilePath;
            }

            DotNetDupe::System::SmartPointer<ILogger> FileLoggerProvider::CreateLogger(const DotNetDupe::System::String& categoryName) {
                return DotNetDupe::System::SmartPointer<FileLogger>::NewShared(categoryName, m_pImpl->config, m_pImpl->context);
            }

        }
    }
}
