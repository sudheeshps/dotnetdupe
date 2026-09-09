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

            /// \brief Central thread-safe static registry and entry point for application logging.
            /// \details Modeled after Microsoft.Extensions.Logging factory patterns and NLog/Log4Net LogManager.
            /// Provides global logger cache, configuration management, and provider registration.
            /// \cite Microsoft.Extensions.Logging
            class LogManager : public DotNetDupe::System::Object {
            public:
                /// \brief Retrieves or creates a cached ILogger instance for the given category name.
                /// \param sCategoryName The logging category name.
                /// \return SmartPointer to the ILogger instance.
                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetLogger(const DotNetDupe::System::String& sCategoryName);

                /// \brief Retrieves a strongly-typed ILoggerOf<T> where the category name is derived from type T.
                /// \tparam T The type used to identify the logger category.
                /// \return SmartPointer to the ILoggerOf<T> instance.
                template <typename T>
                static DotNetDupe::System::SmartPointer<ILoggerOf<T>> GetLogger() {
                    DotNetDupe::System::String sCategoryName = typeid(T).name();
                    return DotNetDupe::System::SmartPointer<Logger<T>>::NewShared(GetLogger(sCategoryName));
                }

                /// \brief Creates a dedicated ConsoleLogger instance for the specified category.
                /// \param sCategoryName The logging category name.
                /// \return SmartPointer to the created ConsoleLogger.
                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetConsoleLogger(const DotNetDupe::System::String& sCategoryName);

                /// \brief Creates a dedicated FileLogger instance for the specified category.
                /// \param sCategoryName The logging category name.
                /// \return SmartPointer to the created FileLogger.
                DOTNETDUPE_API static DotNetDupe::System::SmartPointer<ILogger> GetFileLogger(const DotNetDupe::System::String& sCategoryName);

                /// \brief Sets the global LoggerConfiguration applied across created loggers.
                /// \param config The new LoggerConfiguration settings.
                DOTNETDUPE_API static void Configure(const LoggerConfiguration& config);

                /// \brief Gets the active global LoggerConfiguration.
                /// \return Const reference to the active LoggerConfiguration.
                DOTNETDUPE_API static const LoggerConfiguration& GetConfiguration();

                /// \brief Adds an ILoggerProvider to the internal global LoggerFactory.
                /// \param pProvider SmartPointer to the ILoggerProvider implementation.
                DOTNETDUPE_API static void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider);

                /// \brief Resets the LogManager state, releasing all cached loggers and providers.
                DOTNETDUPE_API static void Reset();
            };

        }
    }
}
