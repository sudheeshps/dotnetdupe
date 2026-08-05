### class `EventLog`

Provides cross-platform interaction with system event logs, supporting native Windows Event Log (`OpenEventLogW`/`ReportEventW`) and POSIX Syslog (`openlog`/`syslog`) on Linux.

#### Enums

##### `enum class EventLogEntryType`

- `Error = 1`: An error event (`EVENTLOG_ERROR_TYPE` on Windows / `LOG_ERR` on Linux).
- `Warning = 2`: A warning event (`EVENTLOG_WARNING_TYPE` on Windows / `LOG_WARNING` on Linux).
- `Information = 4`: An informational event (`EVENTLOG_INFORMATION_TYPE` on Windows / `LOG_INFO` on Linux).
- `SuccessAudit = 8`: A success audit event (`EVENTLOG_AUDIT_SUCCESS` on Windows / `LOG_NOTICE` on Linux).
- `FailureAudit = 16`: A failure audit event (`EVENTLOG_AUDIT_FAILURE` on Windows / `LOG_ALERT` on Linux).

---

#### Methods

##### `static void CreateEventSource(const String& sSource, const String& sLogName)`

Establishes an application source and log name for writing event information.

##### `static bool SourceExists(const String& sSource)`

Determines whether an event source is registered.

##### `static void DeleteEventSource(const String& sSource)`

Removes an event source registration.

##### `static bool Exists(const String& sLogName)`

Determines whether the log exists.

##### `static void Delete(const String& sLogName)`

Removes an event log.

##### `void WriteEntry(const String& sMessage, EventLogEntryType eType = EventLogEntryType::Information, int iEventID = 0)`

Writes an entry to the system event log (Windows EventLog / Linux syslog).

##### `static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType = EventLogEntryType::Information, int iEventID = 0)`

Writes an entry to a system event log using the specified source.

##### `Collections::Generic::List<EventLogEntry> GetEntries() const`

Gets the contents of the event log. On Windows, reads records from `OpenEventLogW`.

##### `void Clear()`

Removes entries from the event log store.

---

## Code Example

The following complete, compilation-ready sample demonstrates creating an event source, writing informational and warning log entries cross-platform (Windows EventLog / Linux Syslog), and reading entries back.

```cpp
#include "System/Console.h"
#include "System/Diagnostics/EventLog.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    String sSource = "MyAppService";
    String sLogName = "Application";

    // 1. Ensure Event Source exists
    if (!EventLog::SourceExists(sSource)) {
        EventLog::CreateEventSource(sSource, sLogName);
    }

    // 2. Instantiate EventLog and write entries to OS event log
    EventLog objLog(sLogName, ".", sSource);
    objLog.WriteEntry("Application started successfully.", EventLogEntryType::Information, 100);
    objLog.WriteEntry("Memory usage is above 85%.", EventLogEntryType::Warning, 201);

    // 3. Read entries
    auto lstEntries = objLog.GetEntries();
    for (int iIdx = 0; iIdx < lstEntries.GetCount(); iIdx++) {
        const auto& entry = lstEntries[iIdx];
        Console::Write("[Event ");
        Console::Write(Convert::ToString(entry.GetInstanceId()));
        Console::Write("] ");
        Console::Write(entry.GetSource());
        Console::Write(": ");
        Console::WriteLine(entry.GetMessage());
    }

    return 0;
}
```
