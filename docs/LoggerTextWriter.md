# LoggerTextWriter

**Namespace:** `DotNetDupe::Extensions::Logging`  
**Header:** `#include "Extensions/Logging/LoggerTextWriter.h"`

Implements `System::IO::TextWriter` to redirect character and line stream output directly into structured logging providers (`ILogger` / `LogManager`) without per-call category lookup overhead.

---

## Syntax

```cpp
class LoggerTextWriter : public DotNetDupe::System::IO::TextWriter;
```

---

## Constructors

### `LoggerTextWriter(const String& sCategoryName = "Console", LogLevel logLevel = LogLevel::Information)`
Initializes a new `LoggerTextWriter` redirecting to the specified logger category and log severity level.

### `LoggerTextWriter(const SmartPointer<ILogger>& pLogger, LogLevel logLevel = LogLevel::Information)`
Initializes a new `LoggerTextWriter` wrapping a concrete `ILogger` instance.

```cpp
auto spWriter = SmartPointer<LoggerTextWriter>::NewShared("AppService", LogLevel::Information);
```

---

## Properties / Methods

### `void Write(const String& sValue) override`
Buffers and writes text to the underlying logger.

### `void WriteLine(const String& sValue) override`
Flushes the line of text immediately to the underlying logger at configured `LogLevel`.

### `void Flush() override`
Flushes any pending output buffers to the logging pipeline.

---

## Example: Redirecting Console to FileLogger

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "Extensions/Logging/LogManager.h"
#include "Extensions/Logging/LoggerTextWriter.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::Extensions::Logging;

int main() {
    // Redirect all Console::WriteLine output into LogManager
    auto spRedirector = SmartPointer<LoggerTextWriter>::NewShared("ConsoleRedirect", LogLevel::Information);
    Console::SetOut(spRedirector);

    // This output is now captured by the logging provider
    Console::WriteLine("Redirected message: Server initialized.");

    return 0;
}
```
