#include "pch.h"
#include "Extensions/Logging/LoggerBase.h"
#include "System/Diagnostics/Process.h"
#include "System/Threading/Thread.h"
#include "System/Text/Json/JsonElement.h"
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

            static void ReplaceDynamicPropertyTokens(std::string& str, const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties, std::vector<std::string>& usedKeys) {
                auto keys = properties.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    DotNetDupe::System::String key = keys[i];
                    std::string placeholder = "{" + std::string(key.GetRawString()) + "}";
                    if (str.find(placeholder) != std::string::npos) {
                        DotNetDupe::System::String val;
                        properties.TryGetValue(key, val);
                        ReplaceToken(str, placeholder, val.GetRawString());
                        usedKeys.push_back(key.GetRawString());
                    }
                }
            }

            static std::string SerializeUnusedPropertiesText(const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties, const std::vector<std::string>& usedKeys) {
                if (properties.GetCount() == 0) return "";
                std::stringstream ss;
                auto keys = properties.GetKeys();
                bool bFirst = true;
                for (int i = 0; i < keys.GetLength(); ++i) {
                    std::string k = keys[i].GetRawString();
                    if (std::find(usedKeys.begin(), usedKeys.end(), k) != usedKeys.end()) continue;
                    if (!bFirst) ss << ", ";
                    DotNetDupe::System::String val;
                    properties.TryGetValue(keys[i], val);
                    ss << k << ": " << val.GetRawString();
                    bFirst = false;
                }
                return ss.str();
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

            static void AddJsonLogProperties(DotNetDupe::System::Text::Json::JsonElement& logObj, const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (properties.GetCount() == 0) return;
                DotNetDupe::System::Text::Json::JsonElement propsObj(DotNetDupe::System::Text::Json::JsonValueKind::Object);
                auto keys = properties.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    DotNetDupe::System::String val;
                    properties.TryGetValue(keys[i], val);
                    propsObj.SetProperty(keys[i], DotNetDupe::System::Text::Json::JsonElement(val));
                }
                logObj.SetProperty("properties", propsObj);
            }

            static DotNetDupe::System::String BuildJsonLog(const DotNetDupe::System::String& timestamp, const char* level, const DotNetDupe::System::String& category, int procId, unsigned long threadId, const DotNetDupe::System::String& message, const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                using namespace DotNetDupe::System::Text::Json;
                JsonElement logObj(JsonValueKind::Object);
                logObj.SetProperty("timestamp", JsonElement(timestamp));
                logObj.SetProperty("level", JsonElement(DotNetDupe::System::String(level)));
                logObj.SetProperty("category", JsonElement(category));
                logObj.SetProperty("processId", JsonElement(static_cast<double>(procId)));
                logObj.SetProperty("threadId", JsonElement(static_cast<double>(threadId)));
                logObj.SetProperty("message", JsonElement(message));
                AddJsonLogProperties(logObj, properties);
                return logObj.ToString();
            }

            DotNetDupe::System::String LoggerBase::BuildLogMessage(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                                    const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) const {
                DotNetDupe::System::String timestamp = GetFormattedTimestamp(m_config.TimestampFormat);
                const char* levelStr = LogLevelToString(logLevel);
                int procId = DotNetDupe::System::Diagnostics::Process::GetCurrentProcessId();
                unsigned long threadId = DotNetDupe::System::Threading::Thread::GetCurrentThreadId();

                if (m_config.IsJsonFormat) {
                    return BuildJsonLog(timestamp, levelStr, m_categoryName, procId, threadId, message, properties);
                }
                std::vector<std::string> usedKeys;
                std::string sWorkingFmt = m_config.PlainTextFormat.GetRawString();
                ReplaceDynamicPropertyTokens(sWorkingFmt, properties, usedKeys);
                std::string propsStr = SerializeUnusedPropertiesText(properties, usedKeys);
                std::string sProcId = std::to_string(procId);
                std::string sThreadId = std::to_string(threadId);
                return FormatLogLine(DotNetDupe::System::String(sWorkingFmt.c_str()), timestamp, DotNetDupe::System::String(levelStr), m_categoryName, message, DotNetDupe::System::String(propsStr.c_str()), DotNetDupe::System::String(sProcId.c_str()), DotNetDupe::System::String(sThreadId.c_str()));
            }

        }
    }
}
