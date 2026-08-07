# LogManager

The `LogManager` class in `DotNetDupe::Extensions::Logging` provides global static factory methods to retrieve, configure, and cache `ConsoleLogger` and `FileLogger` instances by category name or class type.

## Namespace
```cpp
namespace DotNetDupe::Extensions::Logging
```

## Syntax & Class Definition
```cpp
class LogManager : public Object {
public:
    static SmartPointer<ILogger> GetLogger(const String& sCategoryName);

    template <typename T>
    static SmartPointer<ILoggerOf<T>> GetLogger();

    static SmartPointer<ILogger> GetConsoleLogger(const String& sCategoryName);
    static SmartPointer<ILogger> GetFileLogger(const String& sCategoryName);

    static void Configure(const LoggerConfiguration& config);
    static const LoggerConfiguration& GetConfiguration();
    static void AddProvider(const SmartPointer<ILoggerProvider>& pProvider);
    static void Reset();
};
```

## Methods

| Method | Description |
|---|---|
| `GetLogger(sCategoryName)` | Gets a category-cached global logger aggregating configured console & file providers. |
| `GetLogger<T>()` | Strongly-typed generic logger using class `T`'s type name as category. |
| `GetConsoleLogger(category)` | Gets a dedicated category-cached `ConsoleLogger` instance using global configuration. |
| `GetFileLogger(category)` | Gets a dedicated category-cached `FileLogger` instance using global configuration. |
| `Configure(config)` | Sets global `LoggerConfiguration` for all subsequent logger creations. |
| `GetConfiguration()` | Gets the active global `LoggerConfiguration`. |
| `AddProvider(provider)` | Registers a custom `ILoggerProvider` globally. |
| `Reset()` | Clears the global configuration and category logger caches. |

## Code Example

Below is a complete, compile-ready example demonstrating how to retrieve global loggers using `LogManager`.

```cpp
#include "Extensions/Logging/LogManager.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

class PaymentService {};

int main() {
    // 1. One-Liner Global Category Logging
    auto orderLogger = LogManager::GetLogger("OrderService");
    orderLogger->Log(LogLevel::Information, "Order #5001 created.");

    // 2. Strongly-Typed Class Logger
    auto paymentLogger = LogManager::GetLogger<PaymentService>();
    paymentLogger->Log(LogLevel::Information, "Payment authorized for $99.99");

    // 3. Direct Dedicated Console and File Loggers
    auto consoleLogger = LogManager::GetConsoleLogger("UIEvents");
    consoleLogger->Log(LogLevel::Debug, "Button click registered");

    auto fileLogger = LogManager::GetFileLogger("AuditCategory");
    fileLogger->Log(LogLevel::Information, "User password updated.");

    // 4. Redirecting Console Output to Global Logging system via LoggerTextWriter
    auto loggerWriter = SmartPointer<LoggerTextWriter>::NewShared("Console", LogLevel::Information);
    Console::SetOut(loggerWriter);
    Console::WriteLine("This console output is automatically redirected to the logging system!");

    return 0;
}
```
