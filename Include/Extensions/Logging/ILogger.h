#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Defines logging severity levels.
            /// \details Modeled after Microsoft.Extensions.Logging.LogLevel.
            /// \cite Microsoft.Extensions.Logging
            enum class LogLevel {
                /// \brief Logs that contain the most detailed messages.
                Trace = 0,
                /// \brief Logs that are used for interactive investigation during development.
                Debug = 1,
                /// \brief Logs that track the general flow of the application.
                Information = 2,
                /// \brief Logs that highlight an abnormal or unexpected event.
                Warning = 3,
                /// \brief Logs that highlight when the current flow of execution is stopped due to a failure.
                Error = 4,
                /// \brief Logs that describe an unrecoverable application or system crash.
                Critical = 5,
                /// \brief Not used for writing log messages. Specifies that a logging category should not write any messages.
                None = 6
            };

            /// \brief Represents a type used to perform logging.
            /// \details Modeled after Microsoft.Extensions.Logging.ILogger.
            /// Defines core logging methods supporting plain text and structured property dictionaries.
            /// \cite Microsoft.Extensions.Logging
            class ILogger : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~ILogger() = default;

                /// \brief Writes a log entry at the specified log level.
                /// \param logLevel Entry will be written on this level.
                /// \param message The message string to log.
                virtual void Log(LogLevel logLevel, const DotNetDupe::System::String& message) = 0;

                /// \brief Writes a structured log entry with key-value property metadata.
                /// \param logLevel Entry will be written on this level.
                /// \param message The message string to log.
                /// \param properties Dictionary containing structured log properties.
                virtual void Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) = 0;

                /// \brief Checks if the given logLevel is enabled.
                /// \param logLevel Level to be checked.
                /// \return True if enabled; otherwise, false.
                virtual bool IsEnabled(LogLevel logLevel) const = 0;
            };

            /// \brief A generic interface for logging where the category name is derived from the specified type name.
            /// \tparam T The type who's name is used for the logger category name.
            /// \cite Microsoft.Extensions.Logging
            template <typename T>
            class ILoggerOf : public virtual ILogger {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~ILoggerOf() = default;
            };

            /// \brief Represents a type that can create instances of ILogger.
            /// \details Modeled after Microsoft.Extensions.Logging.ILoggerProvider.
            /// \cite Microsoft.Extensions.Logging
            class ILoggerProvider : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~ILoggerProvider() = default;

                /// \brief Creates a new ILogger instance for the specified category.
                /// \param categoryName The category name for messages produced by the logger.
                /// \return SmartPointer to the created ILogger.
                virtual DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) = 0;
            };

            /// \brief Represents a type used to configure the logging system and create instances of ILogger.
            /// \details Modeled after Microsoft.Extensions.Logging.ILoggerFactory.
            /// \cite Microsoft.Extensions.Logging
            class ILoggerFactory : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~ILoggerFactory() = default;

                /// \brief Adds an ILoggerProvider to the logging system.
                /// \param provider The ILoggerProvider to register.
                virtual void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& provider) = 0;

                /// \brief Creates a new ILogger instance for the specified category.
                /// \param categoryName The category name for messages produced by the logger.
                /// \return SmartPointer to the created ILogger.
                virtual DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) = 0;
            };

        }
    }
}
