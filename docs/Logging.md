### Logging Framework

Provides a diagnostics logging infrastructure inspired by `Microsoft.Extensions.Logging` supporting logging levels, multi-provider dispatch, custom log statement layouts, and automatic file rollover/rotation.

#### Namespaces
- `DotNetDupe::Extensions::Logging`

---

### Core Configurations

#### `struct FileRolloverConfig`
Defines settings for size-based log file rotation:
- `bool EnableRollover = false`: Set `true` to enable log rotation.
- `long long MaxFileSizeInBytes = 5 * 1024 * 1024`: Maximum file size (default: 5 MB) before rolling over.
- `int MaxBackupFiles = 3`: Maximum backup files to retain. Older backups are removed automatically.

---

#### `struct LoggerConfiguration`
Central configuration structure for loggers:
- `LogLevel MinLevel = LogLevel::Information`: Minimum severity to log.
- `bool IsJsonFormat = false`: Output structured JSON lines format if `true`.
- `String PlainTextFormat`: The format template for plain text logging. Custom formatting placeholders include:
  - `{Timestamp}`: Formatted time string.
  - `{Level}`: Severity level name.
  - `{Category}`: Log category/scope name.
  - `{Message}`: Main log description.
  - `{Properties}`: Structured properties appended contextually.
- `String TimestampFormat`: Customized timestamp structure using `strftime` syntax (default: `%Y-%m-%d %H:%M:%S`).
- `String FilePath`: Target file path for the file logging provider.
- `FileRolloverConfig Rollover`: Associated log rollover settings.

##### Static Methods
- `static LoggerConfiguration LoadFromFile(const String& filePath)`: Loads the configuration from a JSON file. Throws `IOException` if the file doesn't exist.
- `static LoggerConfiguration LoadFromJson(const String& jsonContent)`: Deserializes the configuration from a JSON string using the library's `JsonSerializer`.

##### JSON Serialization Format & Fallbacks
The parser supports standard JSON serialization of `LoggerConfiguration` properties and `FileRolloverConfig` sub-properties. To make configuration user-friendly, the following fallbacks are supported:
- **Case Insensitivity**: Property names (e.g. `MinLevel`, `IsJsonFormat`, `FilePath`, `Rollover`) can be specified in `PascalCase`, `camelCase`, or `snake_case` (e.g. `min_level`, `is_json_format`, `file_path`, `max_file_size_in_bytes`).
- **LogLevel Representation**: The `MinLevel` log level can be passed as a case-insensitive string (e.g. `"Debug"`, `"warning"`, `"INFO"`, `"Trace"`) or as its corresponding enum integer value (e.g., `0` for Trace, `2` for Information, etc.).

---

### Core Interfaces & Classes

#### `enum class LogLevel`
Specifies severity levels for log entries:
- `Trace = 0` | `Debug = 1` | `Information = 2` | `Warning = 3` | `Error = 4` | `Critical = 5` | `None = 6`

---

#### `class ILogger`
The interface for performing logging operations.

##### Methods
- `void Log(LogLevel logLevel, const String& message)`: Logs a simple text message.
- `void Log(LogLevel logLevel, const String& message, const Dictionary<String, String>& properties)`: Logs a structured message with custom key-value metadata properties.
- `bool IsEnabled(LogLevel logLevel) const`: Checks if the specified log level is enabled.

---

#### `template <typename T> class Logger`
A generic helper class that implements `ILoggerOf<T>` and automatically sets the category name to the type's name.

---

#### `class ILoggerFactory`
Represents a type used to configure the logging system and create instances of `ILogger`.

---

#### `class ConsoleLoggerProvider`
A provider that writes structured logs to the standard output. Exposes constructors taking standard values or custom `LoggerConfiguration` structs.

---

#### `class FileLoggerProvider`
A provider that appends structured log entries to a local log file thread-safely. Supports size-based rotation and rollover.

---

### Complete, Compile-Ready Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/Logger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/ConsoleLoggerProvider.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "System/IO/File.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

class ECommerceService : public Object {
public:
    ECommerceService() = default;
};

int main() {
    auto factory = SmartPointer<LoggerFactory>::NewShared();

    // 1. Configure custom formatted console logger
    LoggerConfiguration consoleConfig;
    consoleConfig.MinLevel = LogLevel::Debug;
    consoleConfig.IsJsonFormat = false;
    // Set custom layout and timestamp formatting
    consoleConfig.PlainTextFormat = "[{Timestamp}] ({Level}) {Category} => {Message} {Properties}";
    consoleConfig.TimestampFormat = "%H:%M:%S";
    
    factory->AddProvider(SmartPointer<ConsoleLoggerProvider>::NewShared(consoleConfig));
    
    // 2. Configure structured JSON file logger with rollover rotation
    LoggerConfiguration fileConfig;
    fileConfig.FilePath = "app_operations.log";
    fileConfig.IsJsonFormat = true;
    fileConfig.MinLevel = LogLevel::Warning;
    fileConfig.Rollover.EnableRollover = true;
    fileConfig.Rollover.MaxFileSizeInBytes = 1000; // Roll over file after 1 KB
    fileConfig.Rollover.MaxBackupFiles = 2; // Keep at most 2 backups
    
    factory->AddProvider(SmartPointer<FileLoggerProvider>::NewShared(fileConfig));

    // 3. Resolve generic logger
    auto innerLogger = factory->CreateLogger("Services::ECommerceService");
    Logger<ECommerceService> logger(innerLogger);

    // 4. Log plain and structured diagnostic messages
    logger.Log(LogLevel::Information, "ECommerce operations start.");

    Collections::Generic::Dictionary<String, String> properties;
    properties.Add("checkoutId", "CK-1094");
    properties.Add("amount", "24.99");
    logger.Log(LogLevel::Warning, "Payment gateway response delayed.", properties);

    return 0;
}
```

#### Loading Configuration from JSON Example

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/ILogger.h"
#include "Extensions/Logging/LoggerConfiguration.h"
#include "Extensions/Logging/LoggerFactory.h"
#include "Extensions/Logging/FileLoggerProvider.h"
#include "System/IO/File.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

int main() {
    // 1. Create a JSON configuration string
    String jsonConfig = "{"
                        "  \"min_level\": \"Debug\","
                        "  \"is_json_format\": true,"
                        "  \"file_path\": \"app_loaded_config.log\","
                        "  \"rollover\": {"
                        "    \"enable_rollover\": true,"
                        "    \"max_file_size_in_bytes\": 1048576,"
                        "    \"max_backup_files\": 5"
                        "  }"
                        "}";

    // 2. Load LoggerConfiguration from the JSON string
    LoggerConfiguration config = LoggerConfiguration::LoadFromJson(jsonConfig);

    // 3. Initialize logger factory and add file provider
    auto factory = SmartPointer<LoggerFactory>::NewShared();
    factory->AddProvider(SmartPointer<FileLoggerProvider>::NewShared(config));

    auto logger = factory->CreateLogger("ECommerceService");
    logger->Log(LogLevel::Information, "Application configuration loaded from JSON successfully.");

    return 0;
}
```
