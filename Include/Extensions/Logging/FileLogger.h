#pragma once
#include "Common.h"
#include "Extensions/Logging/LoggerBase.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            struct FileLoggerContext;

            class FileLogger : public LoggerBase {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                DOTNETDUPE_API FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                          const DotNetDupe::System::SmartPointer<FileLoggerContext>& context);
                DOTNETDUPE_API ~FileLogger() override;

                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message) override;
                DOTNETDUPE_API void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                         const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) override;
            };

        }
    }
}
