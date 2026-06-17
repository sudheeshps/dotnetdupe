#include "pch.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include "System/IO/File.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/Text/Json/JsonException.h"

namespace {
    using namespace DotNetDupe::System;
    using namespace DotNetDupe::System::Text::Json;

    bool TryGetPropertyCaseInsensitive(const JsonElement& element, const String& targetName, JsonElement& outProp) {
        if (element.TryGetProperty(targetName, outProp)) {
            return true;
        }

        auto normalize = [](const String& s) {
            String lower = s.ToLower();
            const char* raw = lower.GetRawString();
            std::string cleaned;
            if (raw) {
                for (int i = 0; raw[i] != '\0'; ++i) {
                    if (raw[i] != '_') {
                        cleaned += raw[i];
                    }
                }
            }
            return String(cleaned.c_str());
        };

        String targetNormalized = normalize(targetName);

        auto propNames = element.GetPropertyNames();
        for (int i = 0; i < propNames.GetLength(); ++i) {
            if (normalize(propNames[i]) == targetNormalized) {
                return element.TryGetProperty(propNames[i], outProp);
            }
        }
        return false;
    }
}

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                template <>
                struct JsonConverter<DotNetDupe::Extensions::Logging::FileRolloverConfig> {
                    static JsonElement Write(const DotNetDupe::Extensions::Logging::FileRolloverConfig& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("EnableRollover", JsonConverter<bool>::Write(value.EnableRollover));
                        obj.SetProperty("MaxFileSizeInBytes", JsonConverter<long long>::Write(value.MaxFileSizeInBytes));
                        obj.SetProperty("MaxBackupFiles", JsonConverter<int>::Write(value.MaxBackupFiles));
                        return obj;
                    }

                    static DotNetDupe::Extensions::Logging::FileRolloverConfig Read(const JsonElement& element) {
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw JsonException("Expected a JSON object for FileRolloverConfig");
                        }
                        DotNetDupe::Extensions::Logging::FileRolloverConfig config;
                        JsonElement prop;
                        if (TryGetPropertyCaseInsensitive(element, "EnableRollover", prop)) {
                            config.EnableRollover = JsonConverter<bool>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "MaxFileSizeInBytes", prop)) {
                            config.MaxFileSizeInBytes = JsonConverter<long long>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "MaxBackupFiles", prop)) {
                            config.MaxBackupFiles = JsonConverter<int>::Read(prop);
                        }
                        return config;
                    }
                };

                template <>
                struct JsonConverter<DotNetDupe::Extensions::Logging::LoggerConfiguration> {
                    static JsonElement Write(const DotNetDupe::Extensions::Logging::LoggerConfiguration& value) {
                        JsonElement obj(JsonValueKind::Object);
                        String minLevelStr;
                        switch (value.MinLevel) {
                            case DotNetDupe::Extensions::Logging::LogLevel::Trace: minLevelStr = "Trace"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Debug: minLevelStr = "Debug"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Information: minLevelStr = "Information"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Warning: minLevelStr = "Warning"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Error: minLevelStr = "Error"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Critical: minLevelStr = "Critical"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::None: minLevelStr = "None"; break;
                            default: minLevelStr = "Information"; break;
                        }
                        obj.SetProperty("MinLevel", JsonConverter<String>::Write(minLevelStr));
                        obj.SetProperty("IsJsonFormat", JsonConverter<bool>::Write(value.IsJsonFormat));
                        obj.SetProperty("PlainTextFormat", JsonConverter<String>::Write(value.PlainTextFormat));
                        obj.SetProperty("TimestampFormat", JsonConverter<String>::Write(value.TimestampFormat));
                        obj.SetProperty("FilePath", JsonConverter<String>::Write(value.FilePath));
                        obj.SetProperty("Rollover", JsonConverter<DotNetDupe::Extensions::Logging::FileRolloverConfig>::Write(value.Rollover));
                        return obj;
                    }

                    static DotNetDupe::Extensions::Logging::LoggerConfiguration Read(const JsonElement& element) {
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw JsonException("Expected a JSON object for LoggerConfiguration");
                        }
                        DotNetDupe::Extensions::Logging::LoggerConfiguration config;
                        JsonElement prop;
                        if (TryGetPropertyCaseInsensitive(element, "MinLevel", prop)) {
                            if (prop.GetValueKind() == JsonValueKind::Number) {
                                int val = JsonConverter<int>::Read(prop);
                                config.MinLevel = static_cast<DotNetDupe::Extensions::Logging::LogLevel>(val);
                            } else if (prop.GetValueKind() == JsonValueKind::String) {
                                config.MinLevel = DotNetDupe::Extensions::Logging::ParseLogLevel(JsonConverter<String>::Read(prop));
                            } else {
                                throw JsonException("Invalid type for MinLevel in LoggerConfiguration");
                            }
                        }
                        if (TryGetPropertyCaseInsensitive(element, "IsJsonFormat", prop)) {
                            config.IsJsonFormat = JsonConverter<bool>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "PlainTextFormat", prop)) {
                            config.PlainTextFormat = JsonConverter<String>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "TimestampFormat", prop)) {
                            config.TimestampFormat = JsonConverter<String>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "FilePath", prop)) {
                            config.FilePath = JsonConverter<String>::Read(prop);
                        }
                        if (TryGetPropertyCaseInsensitive(element, "Rollover", prop)) {
                            config.Rollover = JsonConverter<DotNetDupe::Extensions::Logging::FileRolloverConfig>::Read(prop);
                        }
                        return config;
                    }
                };

            }
        }
    }
}

namespace DotNetDupe {
    namespace Extensions {
        namespace Logging {

            LogLevel ParseLogLevel(const DotNetDupe::System::String& str) {
                DotNetDupe::System::String lower = str.ToLower();
                if (lower == "trace") return LogLevel::Trace;
                if (lower == "debug") return LogLevel::Debug;
                if (lower == "information" || lower == "info") return LogLevel::Information;
                if (lower == "warning" || lower == "warn") return LogLevel::Warning;
                if (lower == "error") return LogLevel::Error;
                if (lower == "critical") return LogLevel::Critical;
                if (lower == "none") return LogLevel::None;

                if (lower == "0") return LogLevel::Trace;
                if (lower == "1") return LogLevel::Debug;
                if (lower == "2") return LogLevel::Information;
                if (lower == "3") return LogLevel::Warning;
                if (lower == "4") return LogLevel::Error;
                if (lower == "5") return LogLevel::Critical;
                if (lower == "6") return LogLevel::None;

                throw DotNetDupe::System::ArgumentException("Invalid LogLevel value");
            }

            LoggerConfiguration LoggerConfiguration::LoadFromFile(const DotNetDupe::System::String& filePath) {
                if (!DotNetDupe::System::IO::File::Exists(filePath)) {
                    throw DotNetDupe::System::IO::IOException("File does not exist");
                }
                DotNetDupe::System::String jsonContent = DotNetDupe::System::IO::File::ReadAllText(filePath);
                return LoadFromJson(jsonContent);
            }

            LoggerConfiguration LoggerConfiguration::LoadFromJson(const DotNetDupe::System::String& jsonContent) {
                return DotNetDupe::System::Text::Json::JsonSerializer::Deserialize<LoggerConfiguration>(jsonContent);
            }

        }
    }
}
