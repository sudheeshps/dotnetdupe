#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include <fstream>
#include <mutex>
#include <memory>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class FileLoggerProvider : public ILoggerProvider {
            private:
                LoggerConfiguration m_config;
                std::shared_ptr<std::ofstream> m_fileStream;
                std::shared_ptr<std::mutex> m_fileMutex;

            public:
                DOTNETDUPE_API FileLoggerProvider(const LoggerConfiguration& config);
                DOTNETDUPE_API FileLoggerProvider(const DotNetDupe::System::String& filePath = "", bool isJsonFormat = false, LogLevel minLevel = LogLevel::Information);
                DOTNETDUPE_API ~FileLoggerProvider() override = default;

                DOTNETDUPE_API const DotNetDupe::System::String& GetFilePath() const { return m_config.FilePath; }

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
