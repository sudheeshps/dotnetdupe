#include "pch.h"
#include "Extensions/Logging/LoggerTextWriter.h"
#include "Extensions/Logging/LogManager.h"
#include "System/Text/TextEncoding.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            LoggerTextWriter::LoggerTextWriter(const DotNetDupe::System::String& sCategoryName, LogLevel logLevel)
                : m_pLogger(LogManager::GetLogger(sCategoryName)), m_logLevel(logLevel) {
            }

            LoggerTextWriter::LoggerTextWriter(const DotNetDupe::System::SmartPointer<ILogger>& pLogger, LogLevel logLevel)
                : m_pLogger(pLogger), m_logLevel(logLevel) {
            }

            void LoggerTextWriter::Close() {
            }

            void LoggerTextWriter::Dispose() {
            }

            void LoggerTextWriter::Flush() {
            }

            System::Text::EncodingPtr LoggerTextWriter::GetEncoding() const {
                return DotNetDupe::System::Text::TextEncoding::UTF8();
            }

            void LoggerTextWriter::Write(bool bValue) {
                Write(DotNetDupe::System::String::Format("{0}", bValue));
            }

            void LoggerTextWriter::Write(char chValue) {
                Write(DotNetDupe::System::String::Format("{0}", chValue));
            }

            void LoggerTextWriter::Write(const char* pValue) {
                if (pValue) {
                    Write(DotNetDupe::System::String(pValue));
                }
            }

            void LoggerTextWriter::Write(const DotNetDupe::System::String& sValue) {
                if (!sValue.IsEmpty() && !m_pLogger.IsNull()) {
                    m_pLogger->Log(m_logLevel, sValue);
                }
            }

            void LoggerTextWriter::Write(int iValue) {
                Write(DotNetDupe::System::String::Format("{0}", iValue));
            }

            void LoggerTextWriter::Write(long long llValue) {
                Write(DotNetDupe::System::String::Format("{0}", llValue));
            }

            void LoggerTextWriter::Write(float fValue) {
                Write(DotNetDupe::System::String::Format("{0}", fValue));
            }

            void LoggerTextWriter::Write(double dValue) {
                Write(DotNetDupe::System::String::Format("{0}", dValue));
            }

            void LoggerTextWriter::WriteLine() {
                Write(DotNetDupe::System::String(""));
            }

            void LoggerTextWriter::WriteLine(bool bValue) {
                Write(bValue);
            }

            void LoggerTextWriter::WriteLine(char chValue) {
                Write(chValue);
            }

            void LoggerTextWriter::WriteLine(const char* pValue) {
                Write(pValue);
            }

            void LoggerTextWriter::WriteLine(const DotNetDupe::System::String& sValue) {
                Write(sValue);
            }

            void LoggerTextWriter::WriteLine(int iValue) {
                Write(iValue);
            }

            void LoggerTextWriter::WriteLine(long long llValue) {
                Write(llValue);
            }

            void LoggerTextWriter::WriteLine(float fValue) {
                Write(fValue);
            }

            void LoggerTextWriter::WriteLine(double dValue) {
                Write(dValue);
            }

        }
    }
}
