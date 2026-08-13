#include "pch.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "System/Console.h"
#include <iostream>

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
                DotNetDupe::System::String formatted = BuildLogMessage(logLevel, message, properties);
                
                // If Console::Out() has been redirected (e.g. to a LoggerTextWriter), write directly to std::cout to avoid an infinite recursion loop
                if (!DotNetDupe::System::Console::Out().IsNull()) {
                    std::cout << formatted.GetRawString() << std::endl;
                } else {
                    DotNetDupe::System::Console::WriteLine(formatted);
                }
            }

        }
    }
}
