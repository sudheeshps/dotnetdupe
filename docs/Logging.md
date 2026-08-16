# Logging, LogManager &amp; LoggerTextWriter

**Namespace:** `DotNetDupe::Extensions::Logging`  
**Header:** `#include "Extensions/Logging/LogManager.h"`, `#include "Extensions/Logging/ILogger.h"`, `#include "Extensions/Logging/LoggerTextWriter.h"`

Provides structured diagnostic logging framework supporting multiple output providers (Console, File, Aggregates), customizable minimum log levels, and automatic stream redirection of `Console::Out` and `Console::Error` via `LoggerTextWriter`.

---

## `LogLevel` Enum

```cpp
enum class LogLevel {
    Trace = 0,
    Debug = 1,
    Information = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
    None = 6
};
```

---

## `ILogger` Interface

### Syntax
```cpp
class ILogger : public virtual Object;
```

### Methods
- `virtual void Log(LogLevel logLevel, const String& message) = 0`
- `virtual void Log(LogLevel logLevel, const String& message, const Dictionary<String, String>& properties) = 0`
- `virtual bool IsEnabled(LogLevel logLevel) const = 0`

---

## `LogManager` Class

Global thread-safe factory and cache for category loggers and provider configurations.

### Static Methods
- `static SmartPointer<ILogger> GetLogger(const String& sCategoryName)`: Returns a category logger (cached).
- `template <typename T> static SmartPointer<ILoggerOf<T>> GetLogger()`: Returns a strongly-typed category logger.
- `static SmartPointer<ILogger> GetConsoleLogger(const String& sCategoryName)`: Creates a console-targeted logger.
- `static SmartPointer<ILogger> GetFileLogger(const String& sCategoryName)`: Creates a file-targeted logger.
- `static void Configure(const LoggerConfiguration& config)`: Configures global minimum levels and file paths.
- `static void AddProvider(const SmartPointer<ILoggerProvider>& pProvider)`: Registers a custom output provider.
- `static void Reset()`: Resets all cached loggers and providers.

---

## `LoggerTextWriter`

Implements `System::IO::TextWriter`, forwarding text stream writes (`Console::WriteLine`, etc.) directly into `LogManager`.

### Constructors
- `LoggerTextWriter(const String& sCategoryName = "Console", LogLevel logLevel = LogLevel::Information)`
- `LoggerTextWriter(const SmartPointer<ILogger>& pLogger, LogLevel logLevel = LogLevel::Information)`

---

## Example

```cpp
#include "System/Console.h"
#include "Extensions/Logging/LogManager.h"
#include "Extensions/Logging/LoggerTextWriter.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

int main() {
    // Structured Logger
    auto pLogger = LogManager::GetLogger("PaymentService");
    pLogger->Log(LogLevel::Information, "Processing transaction #9021");
    pLogger->Log(LogLevel::Warning, "Payment gateway latency is higher than usual.");

    // Redirect standard output to log system
    auto logWriter = SmartPointer<LoggerTextWriter>::NewShared("StdOut", LogLevel::Information);
    Console::SetOut(logWriter);

    Console::WriteLine("This message is automatically routed to LogManager!");

    return 0;
}
```
