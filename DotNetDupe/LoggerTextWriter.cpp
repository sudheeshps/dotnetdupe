#include "pch.h"
#include "Extensions/Logging/LoggerTextWriter.h"
#include "Extensions/Logging/LogManager.h"
#include "System/Text/TextEncoding.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            LoggerTextWriter::LoggerTextWriter(const DotNetDupe::System::String& sCategoryName, LogLevel logLevel)
                : m_pLogger(LogManager::GetLogger(sCategoryName)), m_logLevel(logLevel) {
                /// Resolve target logger from LogManager by category name.
            }

            LoggerTextWriter::LoggerTextWriter(const DotNetDupe::System::SmartPointer<ILogger>& pLogger, LogLevel logLevel)
                : m_pLogger(pLogger), m_logLevel(logLevel) {
                /// Initialize writer wrapping explicit ILogger instance.
            }

            void LoggerTextWriter::Close() {
                /// Flush buffered data and close writer.
                Flush();
            }

            void LoggerTextWriter::Dispose() {
                /// Dispose resources and release logger handle.
                Close();
            }

            void LoggerTextWriter::Flush() {
                /// No-op: Log entries are flushed synchronously per write call.
                (void)0;
            }

            System::Text::EncodingPtr LoggerTextWriter::GetEncoding() const {
                /// Return standard UTF-8 text encoding.
                return DotNetDupe::System::Text::TextEncoding::UTF8();
            }

            void LoggerTextWriter::Write(bool bValue) {
                /// Format boolean and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", bValue));
            }

            void LoggerTextWriter::Write(char chValue) {
                /// Format char and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", chValue));
            }

            void LoggerTextWriter::Write(const char* pValue) {
                /// Guard: Check null pointer and forward.
                if (pValue) {
                    Write(DotNetDupe::System::String(pValue));
                }
            }

            void LoggerTextWriter::Write(const DotNetDupe::System::String& sValue) {
                /// Emit message line to underlying logger.
                if (!sValue.IsEmpty() && !m_pLogger.IsNull()) {
                    m_pLogger->Log(m_logLevel, sValue);
                }
            }

            void LoggerTextWriter::Write(int iValue) {
                /// Format integer and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", iValue));
            }

            void LoggerTextWriter::Write(long long llValue) {
                /// Format 64-bit integer and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", llValue));
            }

            void LoggerTextWriter::Write(float fValue) {
                /// Format float and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", fValue));
            }

            void LoggerTextWriter::Write(double dValue) {
                /// Format double and forward to string write.
                Write(DotNetDupe::System::String::Format("{0}", dValue));
            }

            void LoggerTextWriter::WriteLine() {
                /// Write empty newline entry.
                Write(DotNetDupe::System::String(""));
            }

            void LoggerTextWriter::WriteLine(bool bValue) {
                /// Write boolean line.
                Write(bValue);
            }

            void LoggerTextWriter::WriteLine(char chValue) {
                /// Write char line.
                Write(chValue);
            }

            void LoggerTextWriter::WriteLine(const char* pValue) {
                /// Write C-string line.
                Write(pValue);
            }

            void LoggerTextWriter::WriteLine(const DotNetDupe::System::String& sValue) {
                /// Write String line.
                Write(sValue);
            }

            void LoggerTextWriter::WriteLine(int iValue) {
                /// Write integer line.
                Write(iValue);
            }

            void LoggerTextWriter::WriteLine(long long llValue) {
                /// Write 64-bit integer line.
                Write(llValue);
            }

            void LoggerTextWriter::WriteLine(float fValue) {
                /// Write float line.
                Write(fValue);
            }

            void LoggerTextWriter::WriteLine(double dValue) {
                /// Write double line.
                Write(dValue);
            }

        }
    }
}
