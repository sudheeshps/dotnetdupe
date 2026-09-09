#pragma once
#include "Common.h"
#include "System/IO/TextWriter.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            /// \brief Adapter that routes System::IO::TextWriter stream writes into an ILogger destination.
            /// \details Enables seamless redirection of Console::Out, Console::Error, or other TextWriter streams
            /// into structured LogManager categories with configurable severity levels.
            /// \cite Microsoft.Extensions.Logging
            class LoggerTextWriter : public DotNetDupe::System::IO::TextWriter {
            private:
                DotNetDupe::System::SmartPointer<ILogger> m_pLogger;
                LogLevel m_logLevel;

            public:
                /// \brief Initializes a LoggerTextWriter with category name and log level.
                /// \param sCategoryName The logging category name (default: "Console").
                /// \param logLevel Severity level for written lines (default: Information).
                DOTNETDUPE_API LoggerTextWriter(
                    const DotNetDupe::System::String& sCategoryName = "Console", 
                    LogLevel logLevel = LogLevel::Information);

                /// \brief Initializes a LoggerTextWriter with an explicit ILogger instance.
                /// \param pLogger SmartPointer to the target ILogger.
                /// \param logLevel Severity level for written lines (default: Information).
                DOTNETDUPE_API LoggerTextWriter(
                    const DotNetDupe::System::SmartPointer<ILogger>& pLogger, 
                    LogLevel logLevel = LogLevel::Information);

                /// \brief Virtual destructor.
                DOTNETDUPE_API virtual ~LoggerTextWriter() override = default;

                /// \brief Closes the writer and flushes buffered content.
                DOTNETDUPE_API void Close() override;
                /// \brief Disposes the writer resources.
                DOTNETDUPE_API void Dispose() override;
                /// \brief Flushes any buffered text to the logger.
                DOTNETDUPE_API void Flush() override;

                /// \brief Gets the character encoding for text output (UTF-8).
                DOTNETDUPE_API virtual System::Text::EncodingPtr GetEncoding() const override;

                /// \brief Writes a boolean value to the logger buffer.
                DOTNETDUPE_API void Write(bool bValue) override;
                /// \brief Writes a character to the logger buffer.
                DOTNETDUPE_API void Write(char chValue) override;
                /// \brief Writes a null-terminated C string to the logger buffer.
                DOTNETDUPE_API void Write(const char* pValue) override;
                /// \brief Writes a String to the logger buffer.
                DOTNETDUPE_API void Write(const DotNetDupe::System::String& sValue) override;
                /// \brief Writes a 32-bit integer to the logger buffer.
                DOTNETDUPE_API void Write(int iValue) override;
                /// \brief Writes a 64-bit integer to the logger buffer.
                DOTNETDUPE_API void Write(long long llValue) override;
                /// \brief Writes a single-precision float to the logger buffer.
                DOTNETDUPE_API void Write(float fValue) override;
                /// \brief Writes a double-precision float to the logger buffer.
                DOTNETDUPE_API void Write(double dValue) override;

                /// \brief Writes a line terminator, emitting the accumulated buffer to the logger.
                DOTNETDUPE_API void WriteLine() override;
                /// \brief Writes a boolean followed by a line terminator.
                DOTNETDUPE_API void WriteLine(bool bValue) override;
                /// \brief Writes a character followed by a line terminator.
                DOTNETDUPE_API void WriteLine(char chValue) override;
                /// \brief Writes a C string followed by a line terminator.
                DOTNETDUPE_API void WriteLine(const char* pValue) override;
                /// \brief Writes a String followed by a line terminator.
                DOTNETDUPE_API void WriteLine(const DotNetDupe::System::String& sValue) override;
                /// \brief Writes an integer followed by a line terminator.
                DOTNETDUPE_API void WriteLine(int iValue) override;
                /// \brief Writes a 64-bit integer followed by a line terminator.
                DOTNETDUPE_API void WriteLine(long long llValue) override;
                /// \brief Writes a float followed by a line terminator.
                DOTNETDUPE_API void WriteLine(float fValue) override;
                /// \brief Writes a double followed by a line terminator.
                DOTNETDUPE_API void WriteLine(double dValue) override;
            };

        }
    }
}
