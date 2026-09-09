#include "pch.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "System/Console.h"
#include <iostream>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            ConsoleLogger::ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config)
                : LoggerBase(categoryName, config) {
                /// Initialize base logger with category and configuration.
            }

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                /// Delegate simple message logging to base implementation.
                LoggerBase::Log(logLevel, message);
            }

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                /// Guard: Check if message severity satisfies configured minimum level.
                if (!IsEnabled(logLevel)) return;

                /// Format the log line with timestamps, category, and properties.
                DotNetDupe::System::String sFormatted = BuildLogMessage(logLevel, message, properties);
                
                /// Emit to standard output, bypassing redirected Console::Out to avoid recursion.
                if (!DotNetDupe::System::Console::Out().IsNull()) {
                    std::cout << sFormatted.GetRawString() << std::endl;
                } else {
                    DotNetDupe::System::Console::WriteLine(sFormatted);
                }
            }

        }
    }
}
