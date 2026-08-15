#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class FileLoggerProvider : public ILoggerProvider {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                DOTNETDUPE_API FileLoggerProvider();
                DOTNETDUPE_API FileLoggerProvider(const LoggerConfiguration& config);
                DOTNETDUPE_API FileLoggerProvider(const DotNetDupe::System::String& filePath, bool isJsonFormat = false, LogLevel minLevel = LogLevel::Information);
                DOTNETDUPE_API ~FileLoggerProvider() override;

                DOTNETDUPE_API const DotNetDupe::System::String& GetFilePath() const;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
