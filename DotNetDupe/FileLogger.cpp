#include "pch.h"
#include "Extensions/Logging/FileLogger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdio>

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

            FileLogger::FileLogger(const DotNetDupe::System::String& categoryName, const LoggerConfiguration& config, 
                                   std::shared_ptr<std::ofstream> fileStream, std::shared_ptr<std::mutex> fileMutex)
                : m_categoryName(categoryName), m_config(config), m_fileStream(fileStream), m_fileMutex(fileMutex) {}

            bool FileLogger::IsEnabled(LogLevel logLevel) const {
                return logLevel >= m_config.MinLevel && logLevel != LogLevel::None;
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message) {
                if (!IsEnabled(logLevel)) return;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> emptyProps;
                Log(logLevel, message, emptyProps);
            }

            void FileLogger::Log(LogLevel logLevel, const DotNetDupe::System::String& message, 
                                 const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& properties) {
                if (!IsEnabled(logLevel)) return;
                if (!m_fileStream || !m_fileStream->is_open()) return;

                std::string timestamp = GetFormattedTimestamp(m_config.TimestampFormat.GetRawString());
                const char* levelStr = LogLevelToString(logLevel);
                std::stringstream logLine;

                if (m_config.IsJsonFormat) {
                    logLine << "{\"timestamp\":\"" << timestamp 
                            << "\",\"level\":\"" << levelStr 
                            << "\",\"category\":\"" << m_categoryName.GetRawString() 
                            << "\",\"message\":\"" << message.GetRawString() << "\"";

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

                    std::string formatted = FormatLogLine(
                        m_config.PlainTextFormat.GetRawString(),
                        timestamp,
                        levelStr,
                        m_categoryName.GetRawString(),
                        message.GetRawString(),
                        propsStr
                    );
                    logLine << formatted;
                }

                // Write thread-safely to file
                std::lock_guard<std::mutex> lock(*m_fileMutex);
                std::string line = logLine.str();

                if (m_config.Rollover.EnableRollover && m_fileStream && m_fileStream->is_open()) {
                    auto currentPos = m_fileStream->tellp();
                    if (currentPos != std::streampos(-1)) {
                        long long currentSize = static_cast<long long>(currentPos);
                        if (currentSize + static_cast<long long>(line.length()) >= m_config.Rollover.MaxFileSizeInBytes) {
                            // Close current stream
                            m_fileStream->close();

                            std::string baseName = m_config.FilePath.GetRawString();

                            // Rotate backups: app.log.2 -> app.log.3, etc.
                            for (int i = m_config.Rollover.MaxBackupFiles - 1; i >= 1; --i) {
                                std::string oldBackup = baseName + "." + std::to_string(i);
                                std::string newBackup = baseName + "." + std::to_string(i + 1);
                                std::remove(newBackup.c_str());
                                std::rename(oldBackup.c_str(), newBackup.c_str());
                            }

                            // Rename current file: app.log -> app.log.1
                            std::string firstBackup = baseName + ".1";
                            std::remove(firstBackup.c_str());
                            std::rename(baseName.c_str(), firstBackup.c_str());

                            // Re-open fresh file
                            m_fileStream->open(baseName, std::ios::out | std::ios::app);
                        }
                    }
                }

                if (m_fileStream && m_fileStream->is_open()) {
                    (*m_fileStream) << line << std::endl;
                }
            }

        }
    }
}
