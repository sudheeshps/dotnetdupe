# EtwLogReader &amp; EtwEvent

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/EtwLogReader.h"`

Provides high-performance Event Tracing for Windows (ETW / Windows Event Log channels) and Linux syslog querying, batch event pagination, level counting, and real-time live event streaming subscriptions.

---

## Data Structures & Enums

### `EtwEventLevel`
```cpp
enum class EtwEventLevel {
    All = 0,
    Critical = 1,
    Error = 2,
    Warning = 3,
    Info = 4,
    Verbose = 5
};
```

### `EtwEvent`
```cpp
struct EtwEvent {
    String sChannelName;
    String sMessage;
    int iEventId;
    int iLevel;
    String sProviderName;
    DateTimeOffset dtTimeCreated;
    String sRawXml;
};
```

### `EtwEventLevelCounts`
```cpp
struct EtwEventLevelCounts {
    unsigned long long uCriticalCount;
    unsigned long long uErrorCount;
    unsigned long long uWarningCount;
    unsigned long long uInfoCount;
    unsigned long long uVerboseCount;
};
```

---

## `EtwLogReader` Class

### Syntax
```cpp
class EtwLogReader : public Object;
```

---

## Member Functions (Live Subscriptions)

### `void StartListening(const String& sChannelName, Action<const EtwEvent&> fnCallback)`
Starts asynchronous real-time event listening on the specified channel. Invokes `fnCallback` whenever a new event occurs.

### `void StopListening()`
Stops real-time event listening on the active channel and releases subscription resources.

### `bool IsListening() const`
Gets whether the instance is currently actively subscribed to live events.

### `String GetListeningChannel() const`
Gets the name of the channel currently being monitored.

---

## Static Methods

### `static List<String> GetEventChannels()`
Enumerates all registered ETW / Event Log channels on the operating system.

### `static unsigned long long GetChannelEventCount(const String& sChannelName)`
Gets the total count of recorded events in a specific channel.

### `static EtwEventLevelCounts GetChannelEventLevelCounts(const String& sChannelName)`
Retrieves an aggregate count of events partitioned by severity level (Critical, Error, Warning, Info, Verbose).

### `static List<EtwEvent> ReadEvents(...)`
Queries and parses records from the specified channel with pagination, reverse ordering, and severity level filtering.

Overloads:
- `static List<EtwEvent> ReadEvents(const String& sChannelName)`
- `static List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents)`
- `static List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex)`
- `static List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection)`
- `static List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level)`

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/EtwLogReader.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    String sChannel = "Application";

    // Query event stats
    unsigned long long uTotal = EtwLogReader::GetChannelEventCount(sChannel);
    Console::WriteLine("Total events in '{0}': {1}", sChannel, (long long)uTotal);

    // Read top 5 latest error events
    auto listEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Error);
    Console::WriteLine("Read {0} recent error events:", listEvents.GetCount());
    for (int iIdx = 0; iIdx < listEvents.GetCount(); ++iIdx) {
        Console::WriteLine(" [{0}] ID:{1} From:{2} - {3}", 
            listEvents[iIdx].dtTimeCreated.ToString(),
            listEvents[iIdx].iEventId, 
            listEvents[iIdx].sProviderName,
            listEvents[iIdx].sMessage);
    }

    return 0;
}
```
