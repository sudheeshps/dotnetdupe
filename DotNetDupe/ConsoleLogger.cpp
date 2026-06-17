#include "pch.h"
#include "Extensions/Logging/ConsoleLogger.h"
#include "System/Console.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            namespace {
                std::string GetFormattedTimestamp(const std::string& formatFmt) {
                    auto now = std::chrono::system_clock::now();
                    auto time = std::chrono::system_clock::to_time_t(now);
                    struct tm buf;
#if defined(_WIN32)
                    localtime_s(&buf, &time);
#else
                    localtime_r(&time, &buf);
#endif
                    char timeStr[100];
                    size_t written = std::strftime(timeStr, sizeof(timeStr), formatFmt.c_str(), &buf);
                    if (written > 0) {
                        return std::string(timeStr);
                    }
                    return "time_error";
                }

                const char* LogLevelToString(LogLevel level) {
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

                std::string FormatLogLine(const std::string& fmt, const std::string& timestamp, const std::string& level, const std::string& category, const std::string& message, const std::string& properties) {
                    std::string res = fmt;
                    auto replace = [](std::string& str, const std::string& from, const std::string& to) {
                        size_t start_pos = 0;
                        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                            str.replace(start_pos, from.length(), to);
                            start_pos += to.length();
                        }
                    };
                    replace(res, "{Timestamp}", timestamp);
                    replace(res, "{Level}", level);
                    replace(res, "{Category}", category);
                    replace(res, "{Message}", message);
                    replace(res, "{Properties}", properties);
                    return res;
                }
            }

            ConsoleLogger::ConsoleLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config)
                : m_categoryName(categoryName), m_config(config) {}

            bool ConsoleLogger::IsEnabled(LogLevel logLevel) const {
                return logLevel >= m_config.MinLevel && logLevel != LogLevel::None;
            }

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                if (!IsEnabled(logLevel)) return;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> emptyProps;
                Log(logLevel, message, emptyProps);
            }

            void ConsoleLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                     const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;

                std::string timestamp = GetFormattedTimestamp(m_config.TimestampFormat.GetRawString());
                const char* levelStr = LogLevelToString(logLevel);

                if (m_config.IsJsonFormat) {
                    std::stringstream ss;
                    ss << "{\"timestamp\":\"" << timestamp 
                       << "\",\"level\":\"" << levelStr 
                       << "\",\"category\":\"" << m_categoryName.GetRawString() 
                       << "\",\"message\":\"" << message.GetRawString() << "\"";

                    if (properties.GetCount() > 0) {
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
                    }
                    ss << "}";
                    DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String(ss.str().c_str()));
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

                    std::string formatted = FormatLogLine(
                        m_config.PlainTextFormat.GetRawString(),
                        timestamp,
                        levelStr,
                        m_categoryName.GetRawString(),
                        message.GetRawString(),
                        propsStr
                    );

                    DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String(formatted.c_str()));
                }
            }

        }
    }
}
