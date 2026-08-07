#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/Logger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include <typeinfo>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            class LogManager : public DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetLogger(const DotNetDupe::System::String& sCategoryName);

                template <typename T>
                static DotNetDupe::System::SmartPointer<ILoggerOf<T>> GetLogger() {
                    DotNetDupe::System::String sCategoryName = typeid(T).name();
                    return DotNetDupe::System::SmartPointer<Logger<T>>::NewShared(GetLogger(sCategoryName));
                }

                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetConsoleLogger(const DotNetDupe::System::String& sCategoryName);
                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetFileLogger(const DotNetDupe::System::String& sCategoryName);

                DOTNETDUPE_API static void Configure(const LoggerConfiguration& config);
                DOTNETDUPE_API static const LoggerConfiguration& GetConfiguration();
                DOTNETDUPE_API static void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider);
                DOTNETDUPE_API static void Reset();
            };

        }
    }
}
