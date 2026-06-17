#pragma once
#include "Common.h"
#include "Extensions/Logging/LoggerBase.h"
#include <fstream>
#include <mutex>
#include <memory>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class FileLogger : public LoggerBase {
            private:
                std::shared_ptr<std::ofstream> m_fileStream;
                std::shared_ptr<std::mutex> m_fileMutex;

            public:
                DOTNETDUPE_API FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                          std::shared_ptr<std::ofstream> fileStream, std::shared_ptr<std::mutex> fileMutex);
                DOTNETDUPE_API ~FileLogger() override = default;

                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
