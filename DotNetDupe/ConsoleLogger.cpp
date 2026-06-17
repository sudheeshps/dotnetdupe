#include "pch.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "System/Console.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            ConsoleLogger::ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config)
                : LoggerBase(categoryName, config) {}

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                LoggerBase::Log(logLevel, message);
            }

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;
                std::string formatted = BuildLogMessage(logLevel, message, properties);
                DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String(formatted.c_str()));
            }

        }
    }
}
