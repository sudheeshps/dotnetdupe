#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            enum class LogLevel {
                Trace = 0,
                Debug = 1,
                Information = 2,
                Warning = 3,
                Error = 4,
                Critical = 5,
                None = 6
            };

            class ILogger : public virtual DotNetDupe::System::Object {
            public:
                virtual ~ILogger() = default;
                virtual void Log(LogLevel logLevel, const DotNetDupe::System::String& message) = 0;
                virtual void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) = 0;
                virtual bool IsEnabled(LogLevel logLevel) const = 0;
            };

            template <typename T>
            class ILoggerOf : public virtual ILogger {
            public:
                virtual ~ILoggerOf() = default;
            };

            class ILoggerProvider : public virtual DotNetDupe::System::Object {
            public:
                virtual ~ILoggerProvider() = default;
                virtual DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) = 0;
            };

            class ILoggerFactory : public virtual DotNetDupe::System::Object {
            public:
                virtual ~ILoggerFactory() = default;
                virtual void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& provider) = 0;
                virtual DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) = 0;
            };

        }
    }
}
