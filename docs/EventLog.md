# EventLog &amp; EventLogEntry

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/EventLog.h"`

Provides interaction with system event logs, enabling reading and writing of operational diagnostic events across Windows (Windows Event Log) and Linux (`syslog`).

---

## `EventLogEntryType` Enum

```cpp
enum class EventLogEntryType {
    Error = 1,
    Warning = 2,
    Information = 4,
    SuccessAudit = 8,
    FailureAudit = 16
};
```

---

## `EventLogEntry` Class

Encapsulates an individual recorded event log record.

### Syntax
```cpp
class EventLogEntry : public Object;
```

### Constructors
- `EventLogEntry()`: Initializes an empty event log entry.
- `EventLogEntry(const String& sMessage, EventLogEntryType eType, int iInstanceId, const String& sSource, const DateTimeOffset& dtTimeGenerated)`: Initializes an event log entry with full metadata.

### Properties
- `String GetMessage() const`: Gets the event description text.
- `EventLogEntryType GetEntryType() const`: Gets the severity level of the event entry.
- `int GetInstanceId() const`: Gets the numeric event identifier.
- `String GetSource() const`: Gets the name of the application source that registered the event.
- `DateTimeOffset GetTimeGenerated() const`: Gets the date and time that the event was recorded.

---

## `EventLog` Class

### Syntax
```cpp
class EventLog : public Object;
```

---

## Constructors

### `EventLog()`
Initializes an empty instance of the `EventLog` class.

### `EventLog(const String& sLogName)`
Initializes an instance targeting a specific log channel (e.g. `"Application"`, `"System"`).

### `EventLog(const String& sLogName, const String& sMachineName)`
Initializes an instance targeting a specific log on a given machine name.

### `EventLog(const String& sLogName, const String& sMachineName, const String& sSource)`
Initializes an instance targeting a log with an explicit application source identifier.

---

## Properties

- `String GetLog() const` / `void SetLog(const String& sLogName)`: Gets or sets the name of the log.
- `String GetMachineName() const` / `void SetMachineName(const String& sMachineName)`: Gets or sets the machine name.
- `String GetSource() const` / `void SetSource(const String& sSource)`: Gets or sets the source name.

---

## Member Functions

### `List<EventLogEntry> GetEntries() const`
Reads and returns all entries contained in the targeted event log channel.

### `void WriteEntry(const String& sMessage)`
### `void WriteEntry(const String& sMessage, EventLogEntryType eType)`
### `void WriteEntry(const String& sMessage, EventLogEntryType eType, int iEventID)`
Writes an entry into the event log instance with the specified message, severity level, and event ID.

### `void Clear()`
Removes all entries from the event log.

### `void Close()`
Closes the event log handle and releases resources.

---

## Static Methods

### `static void WriteEntry(const String& sSource, const String& sMessage)`
### `static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType)`
### `static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID)`
Writes an entry directly into the system event log using a static helper.

### `static bool SourceExists(const String& sSource)` / `static bool SourceExists(const String& sSource, const String& sMachineName)`
Determines whether an event source is registered on the computer.

### `static void CreateEventSource(const String& sSource, const String& sLogName)`
Establishes a valid event source for writing localized event messages.

### `static void Delete(const String& sLogName)` / `static void Delete(const String& sLogName, const String& sMachineName)`
Removes an event log from the computer.

### `static void DeleteEventSource(const String& sSource)` / `static void DeleteEventSource(const String& sSource, const String& sMachineName)`
Removes the event source registration.

### `static bool Exists(const String& sLogName)` / `static bool Exists(const String& sLogName, const String& sMachineName)`
Determines whether the specified log exists on the computer.

### `static List<EventLog> GetEventLogs()` / `static List<EventLog> GetEventLogs(const String& sMachineName)`
Enumerates all available event logs on the machine.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/EventLog.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    String sSourceName = "DotNetDupeApp";

    if (!EventLog::SourceExists(sSourceName)) {
        EventLog::CreateEventSource(sSourceName, "Application");
    }

    // Write diagnostic records
    EventLog::WriteEntry(sSourceName, "Service started successfully.", EventLogEntryType::Information, 1001);
    EventLog::WriteEntry(sSourceName, "High memory pressure detected.", EventLogEntryType::Warning, 2002);

    EventLog logApp("Application");
    auto listEntries = logApp.GetEntries();
    Console::WriteLine("Total entries read: {0}", listEntries.GetCount());

    return 0;
}
```
