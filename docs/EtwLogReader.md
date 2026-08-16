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

### `static List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents = 100, int iStartIndex = 0, bool bReverseDirection = true, EtwEventLevel level = EtwEventLevel::All)`
Queries and parses records from the specified channel with pagination, reverse ordering, and severity level filtering.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/EtwLogReader.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    String channel = "Application";

    // Query event stats
    unsigned long long total = EtwLogReader::GetChannelEventCount(channel);
    Console::WriteLine("Total events in '{0}': {1}", channel, (long long)total);

    // Read top 5 latest error events
    auto events = EtwLogReader::ReadEvents(channel, 5, 0, true, EtwEventLevel::Error);
    Console::WriteLine("Read {0} recent error events:", events.GetCount());
    for (int i = 0; i < events.GetCount(); ++i) {
        Console::WriteLine(" [{0}] ID:{1} From:{2} - {3}", 
            events[i].dtTimeCreated.ToString(),
            events[i].iEventId, 
            events[i].sProviderName,
            events[i].sMessage);
    }

    return 0;
}
```
