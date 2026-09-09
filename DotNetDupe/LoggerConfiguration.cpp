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
        /// Check exact property match first.
        if (element.TryGetProperty(targetName, outProp)) {
            return true;
        }

        /// Normalize identifier strings by stripping underscores and converting to lowercase.
        auto normalize = [](const String& s) {
            String lower = s.ToLower();
            const char* raw = lower.GetRawString();
            std::string cleaned;
            if (raw) {
                for (int iIdx = 0; raw[iIdx] != '\0'; ++iIdx) {
                    if (raw[iIdx] != '_') {
                        cleaned += raw[iIdx];
                    }
                }
            }
            return String(cleaned.c_str());
        };

        String targetNormalized = normalize(targetName);

        /// Scan all property names for normalized match.
        auto propNames = element.GetPropertyNames();
        for (int iIdx = 0; iIdx < propNames.GetLength(); ++iIdx) {
            if (normalize(propNames[iIdx]) == targetNormalized) {
                return element.TryGetProperty(propNames[iIdx], outProp);
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
                        /// Serialize FileRolloverConfig fields to JsonElement.
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("EnableRollover", JsonConverter<bool>::Write(value.EnableRollover));
                        obj.SetProperty("MaxFileSizeInBytes", JsonConverter<long long>::Write(value.MaxFileSizeInBytes));
                        obj.SetProperty("MaxBackupFiles", JsonConverter<int>::Write(value.MaxBackupFiles));
                        return obj;
                    }

                    static DotNetDupe::Extensions::Logging::FileRolloverConfig Read(const JsonElement& element) {
                        /// Guard: Validate element is a JSON object.
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw JsonException("Expected a JSON object for FileRolloverConfig");
                        }
                        DotNetDupe::Extensions::Logging::FileRolloverConfig config;
                        JsonElement prop;

                        /// Deserialize rollover configuration options case-insensitively.
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
                        /// Map LogLevel to string representation.
                        JsonElement obj(JsonValueKind::Object);
                        const char* pMinLevelStr = "Information";
                        switch (value.MinLevel) {
                            case DotNetDupe::Extensions::Logging::LogLevel::Trace: pMinLevelStr = "Trace"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Debug: pMinLevelStr = "Debug"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Information: pMinLevelStr = "Information"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Warning: pMinLevelStr = "Warning"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Error: pMinLevelStr = "Error"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::Critical: pMinLevelStr = "Critical"; break;
                            case DotNetDupe::Extensions::Logging::LogLevel::None: pMinLevelStr = "None"; break;
                            default: break;
                        }

                        /// Serialize LoggerConfiguration properties to JsonElement.
                        obj.SetProperty("MinLevel", JsonConverter<String>::Write(String(pMinLevelStr)));
                        obj.SetProperty("IsJsonFormat", JsonConverter<bool>::Write(value.IsJsonFormat));
                        obj.SetProperty("PlainTextFormat", JsonConverter<String>::Write(value.PlainTextFormat));
                        obj.SetProperty("TimestampFormat", JsonConverter<String>::Write(value.TimestampFormat));
                        obj.SetProperty("FilePath", JsonConverter<String>::Write(value.FilePath));
                        obj.SetProperty("Rollover", JsonConverter<DotNetDupe::Extensions::Logging::FileRolloverConfig>::Write(value.Rollover));
                        return obj;
                    }

                    static DotNetDupe::Extensions::Logging::LoggerConfiguration Read(const JsonElement& element) {
                        /// Guard: Validate element is a JSON object.
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw JsonException("Expected a JSON object for LoggerConfiguration");
                        }
                        DotNetDupe::Extensions::Logging::LoggerConfiguration config;
                        JsonElement prop;

                        /// Read MinLevel supporting string name or integer value.
                        if (TryGetPropertyCaseInsensitive(element, "MinLevel", prop)) {
                            if (prop.GetValueKind() == JsonValueKind::Number) {
                                config.MinLevel = static_cast<DotNetDupe::Extensions::Logging::LogLevel>(JsonConverter<int>::Read(prop));
                            } else if (prop.GetValueKind() == JsonValueKind::String) {
                                config.MinLevel = DotNetDupe::Extensions::Logging::ParseLogLevel(JsonConverter<String>::Read(prop));
                            } else {
                                throw JsonException("Invalid type for MinLevel in LoggerConfiguration");
                            }
                        }

                        /// Read format templates and output paths.
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
                /// Normalize input string to lowercase for case-insensitive comparison.
                DotNetDupe::System::String sLower = str.ToLower();

                /// Lookup level against known string names and numeric codes (CCN <= 4).
                static const struct { const char* pName; LogLevel eLevel; } s_mappings[] = {
                    { "trace", LogLevel::Trace }, { "0", LogLevel::Trace },
                    { "debug", LogLevel::Debug }, { "1", LogLevel::Debug },
                    { "information", LogLevel::Information }, { "info", LogLevel::Information }, { "2", LogLevel::Information },
                    { "warning", LogLevel::Warning }, { "warn", LogLevel::Warning }, { "3", LogLevel::Warning },
                    { "error", LogLevel::Error }, { "4", LogLevel::Error },
                    { "critical", LogLevel::Critical }, { "5", LogLevel::Critical },
                    { "none", LogLevel::None }, { "6", LogLevel::None }
                };

                for (const auto& mapping : s_mappings) {
                    if (sLower == mapping.pName) return mapping.eLevel;
                }

                /// Throw custom library exception if input is invalid.
                throw DotNetDupe::System::ArgumentException("Invalid LogLevel value");
            }

            LoggerConfiguration LoggerConfiguration::LoadFromFile(const DotNetDupe::System::String& filePath) {
                /// Guard: Verify file existence before reading.
                if (!DotNetDupe::System::IO::File::Exists(filePath)) {
                    throw DotNetDupe::System::IO::IOException("File does not exist");
                }

                /// Read entire file content into string and deserialize.
                DotNetDupe::System::String sJsonContent = DotNetDupe::System::IO::File::ReadAllText(filePath);
                return LoadFromJson(sJsonContent);
            }

            LoggerConfiguration LoggerConfiguration::LoadFromJson(const DotNetDupe::System::String& jsonContent) {
                /// Deserialize LoggerConfiguration from JSON string.
                return DotNetDupe::System::Text::Json::JsonSerializer::Deserialize<LoggerConfiguration>(jsonContent);
            }

        }
    }
}

