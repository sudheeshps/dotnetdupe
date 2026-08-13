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

            DotNetDupe::System::String LoggerBase::FormatLogLine(const DotNetDupe::System::String& fmt, const DotNetDupe::System::String& timestamp, const DotNetDupe::System::String& level, const DotNetDupe::System::String& category, const DotNetDupe::System::String& message, const DotNetDupe::System::String& properties, const DotNetDupe::System::String& processId, const DotNetDupe::System::String& threadId) const {
                std::string res = fmt.GetRawString();
                auto replace = [](std::string& str, const std::string& from, const std::string& to) {
                    size_t start_pos = 0;
                    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                        str.replace(start_pos, from.length(), to);
                        start_pos += to.length();
                    }
                };
                replace(res, "{Timestamp}", timestamp.GetRawString());
                replace(res, "{Level}", level.GetRawString());
                replace(res, "{Category}", category.GetRawString());
                replace(res, "{Message}", message.GetRawString());
                replace(res, "{Properties}", properties.GetRawString());
                replace(res, "{ProcessId}", processId.GetRawString());
                replace(res, "{ThreadId}", threadId.GetRawString());
                return DotNetDupe::System::String(res.c_str());
            }

            DotNetDupe::System::String LoggerBase::BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                                    const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) const {
                DotNetDupe::System::String timestamp = GetFormattedTimestamp(m_config.TimestampFormat);
                const char* levelStr = LogLevelToString(logLevel);
                std::string procId = std::to_string(DotNetDupe::System::Diagnostics::Process::GetCurrentProcessId());
                std::string threadId = std::to_string(DotNetDupe::System::Threading::Thread::GetCurrentThreadId());

                std::stringstream logLine;

                if (m_config.IsJsonFormat) {
                    logLine << "{\"timestamp\":\"" << timestamp.GetRawString() 
                            << "\",\"level\":\"" << levelStr 
                            << "\",\"category\":\"" << m_categoryName.GetRawString() 
                            << "\",\"processId\":" << procId
                            << ",\"threadId\":" << threadId
                            << ",\"message\":\"" << message.GetRawString() << "\"";

                    if (properties.GetCount() > 0) {
                        logLine << ",\"properties\":{";
                        auto keys = properties.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            if (i > 0) logLine << ",";
                            DotNetDupe::System::String key = keys[i];
                            DotNetDupe::System::String val;
                            properties.TryGetValue(key, val);
                            logLine << "\"" << key.GetRawString() << "\":\"" << val.GetRawString() << "\"";
                        }
                        logLine << "}";
                    }
                    logLine << "}";
                } else {
                    std::string propsStr = "";
                    if (properties.GetCount() > 0) {
                        std::stringstream ss;
                        auto keys = properties.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            if (i > 0) ss << ", ";
                            DotNetDupe::System::String key = keys[i];
                            DotNetDupe::System::String val;
                            properties.TryGetValue(key, val);
                            ss << key.GetRawString() << ": " << val.GetRawString();
                        }
                        propsStr = ss.str();
                    }

                    DotNetDupe::System::String formatted = FormatLogLine(
                        m_config.PlainTextFormat,
                        timestamp,
                        DotNetDupe::System::String(levelStr),
                        m_categoryName,
                        message,
                        DotNetDupe::System::String(propsStr.c_str()),
                        DotNetDupe::System::String(procId.c_str()),
                        DotNetDupe::System::String(threadId.c_str())
                    );
                    logLine << formatted.GetRawString();
                }

                return DotNetDupe::System::String(logLine.str().c_str());
            }

        }
    }
}
