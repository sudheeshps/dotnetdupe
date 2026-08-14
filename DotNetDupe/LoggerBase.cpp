#include "pch.h"
#include "Extensions/Logging/LoggerBase.h"
#include "System/Diagnostics/Process.h"
#include "System/Threading/Thread.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            LoggerBase::LoggerBase(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config)
                : m_categoryName(categoryName), m_config(config) {}

            bool LoggerBase::IsEnabled(LogLevel logLevel) const {
                return logLevel >= m_config.MinLevel && logLevel != LogLevel::None;
            }

            void LoggerBase::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                if (!IsEnabled(logLevel)) return;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> emptyProps;
                Log(logLevel, message, emptyProps);
            }

            DotNetDupe::System::String LoggerBase::GetFormattedTimestamp(const DotNetDupe::System::String& formatFmt) const {
                auto now = std::chrono::system_clock::now();
                auto time = std::chrono::system_clock::to_time_t(now);
                struct tm buf;
#if defined(_WIN32)
                gmtime_s(&buf, &time);
#else
                gmtime_r(&time, &buf);
#endif
                std::string fmt = formatFmt.IsEmpty() ? "%Y-%m-%d %H:%M:%S" : formatFmt.GetRawString();
                char timeStr[255] = { 0 };
                size_t written = std::strftime(timeStr, sizeof(timeStr), fmt.c_str(), &buf);
                if (written > 0) {
                    return DotNetDupe::System::String(timeStr);
                }
                return DotNetDupe::System::String("time_error");
            }

            const char* LoggerBase::LogLevelToString(LogLevel level) const {
                switch (level) {
                    case LogLevel::Trace: return "Trace";
                    case LogLevel::Debug: return "Debug";
                    case LogLevel::Information: return "Information";
                    case LogLevel::Warning: return "Warning";
                    case LogLevel::Error: return "Error";
                    case LogLevel::Critical: return "Critical";
                    default: return "None";
                }
            }

            static void ReplaceToken(std::string& str, const std::string& from, const char* to) {
                size_t pos = 0;
                std::string sTo = to ? to : "";
                while ((pos = str.find(from, pos)) != std::string::npos) {
                    str.replace(pos, from.length(), sTo);
                    pos += sTo.length();
                }
            }

            DotNetDupe::System::String LoggerBase::FormatLogLine(const DotNetDupe::System::String& fmt, const DotNetDupe::System::String& timestamp, const DotNetDupe::System::String& level, const DotNetDupe::System::String& category, const DotNetDupe::System::String& message, const DotNetDupe::System::String& properties, const DotNetDupe::System::String& processId, const DotNetDupe::System::String& threadId) const {
                std::string res = fmt.GetRawString();
                ReplaceToken(res, "{Timestamp}", timestamp.GetRawString());
                ReplaceToken(res, "{Level}", level.GetRawString());
                ReplaceToken(res, "{Category}", category.GetRawString());
                ReplaceToken(res, "{Message}", message.GetRawString());
                ReplaceToken(res, "{Properties}", properties.GetRawString());
                ReplaceToken(res, "{ProcessId}", processId.GetRawString());
                ReplaceToken(res, "{ThreadId}", threadId.GetRawString());
                return DotNetDupe::System::String(res.c_str());
            }

            static std::string SerializePropertiesJson(const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (properties.GetCount() == 0) return "";
                std::stringstream ss;
                ss << ",\"properties\":{";
                auto keys = properties.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    if (i > 0) ss << ",";
                    DotNetDupe::System::String key = keys[i];
                    DotNetDupe::System::String val;
                    properties.TryGetValue(key, val);
                    ss << "\"" << key.GetRawString() << "\":\"" << val.GetRawString() << "\"";
                }
                ss << "}";
                return ss.str();
            }

            static std::string SerializePropertiesText(const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (properties.GetCount() == 0) return "";
                std::stringstream ss;
                auto keys = properties.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    if (i > 0) ss << ", ";
                    DotNetDupe::System::String key = keys[i];
                    DotNetDupe::System::String val;
                    properties.TryGetValue(key, val);
                    ss << key.GetRawString() << ": " << val.GetRawString();
                }
                return ss.str();
            }

            static DotNetDupe::System::String BuildJsonLog(const char* timestamp, const char* level, const char* category, const std::string& procId, const std::string& threadId, const char* message, const std::string& jsonProps) {
                std::stringstream logLine;
                logLine << "{\"timestamp\":\"" << timestamp 
                        << "\",\"level\":\"" << level 
                        << "\",\"category\":\"" << category 
                        << "\",\"processId\":" << procId
                        << ",\"threadId\":" << threadId
                        << ",\"message\":\"" << message << "\""
                        << jsonProps << "}";
                return DotNetDupe::System::String(logLine.str().c_str());
            }

            DotNetDupe::System::String LoggerBase::BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                                    const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) const {
                DotNetDupe::System::String timestamp = GetFormattedTimestamp(m_config.TimestampFormat);
                const char* levelStr = LogLevelToString(logLevel);
                std::string procId = std::to_string(DotNetDupe::System::Diagnostics::Process::GetCurrentProcessId());
                std::string threadId = std::to_string(DotNetDupe::System::Threading::Thread::GetCurrentThreadId());

                if (m_config.IsJsonFormat) {
                    std::string jsonProps = SerializePropertiesJson(properties);
                    return BuildJsonLog(timestamp.GetRawString(), levelStr, m_categoryName.GetRawString(), procId, threadId, message.GetRawString(), jsonProps);
                }
                std::string propsStr = SerializePropertiesText(properties);
                return FormatLogLine(m_config.PlainTextFormat, timestamp, DotNetDupe::System::String(levelStr), m_categoryName, message, DotNetDupe::System::String(propsStr.c_str()), DotNetDupe::System::String(procId.c_str()), DotNetDupe::System::String(threadId.c_str()));
            }

        }
    }
}
