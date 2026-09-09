#pragma once
#include "Common.h"
#include "Extensions/Logging/ILogger.h"


namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Factory class used to configure logging providers and create ILogger instances.
            /// \details Modeled after Microsoft.Extensions.Logging.LoggerFactory.
            /// Supports registering multiple ILoggerProvider backends (e.g. Console, File)
            /// and creates composite AggregateLogger instances dispatching to all registered providers.
            /// \cite Microsoft.Extensions.Logging
            class LoggerFactory : public ILoggerFactory {
            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

            public:
                /// \brief Initializes a new LoggerFactory instance.
                DOTNETDUPE_API LoggerFactory();

                /// \brief Virtual destructor cleaning up registered providers.
                DOTNETDUPE_API ~LoggerFactory() override;

                /// \brief Adds an ILoggerProvider backend to the factory.
                /// \param pProvider SmartPointer to the ILoggerProvider implementation.
                DOTNETDUPE_API void AddProvider(const DotNetDupe::System::SmartPointer<ILoggerProvider>& pProvider) override;

                /// \brief Creates a new ILogger instance for the specified category.
                /// \param categoryName The category name for messages produced by the logger.
                /// \return SmartPointer to an ILogger (or AggregateLogger if multiple providers are registered).
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<ILogger> CreateLogger(const DotNetDupe::System::String& categoryName) override;
            };

        }
    }
}
