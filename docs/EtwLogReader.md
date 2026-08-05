### class `EtwLogReader`

Provides Event Tracing for Windows (ETW) and Linux Syslog channel enumeration, event reading, and live subscription listener capabilities.

#### Structs

##### `struct EtwEvent`

- `String sChannelName`: Name of the channel.
- `String sMessage`: Formatted event message payload.
- `int iEventId`: Unique event identifier.
- `int iLevel`: Severity level (e.g. Critical, Error, Warning, Information, Verbose).
- `String sProviderName`: Event provider name.
- `DateTimeOffset dtTimeCreated`: Timestamp when event was generated.
- `String sRawXml`: Complete raw XML payload of the event rendered via native `EvtRender`.

---

#### Methods

##### `static Collections::Generic::List<String> GetEventChannels()`

Enumerates available event channels on Windows (`EvtOpenChannelEnum`/`EvtNextChannelPath`) or system log categories on Linux (`syslog`, `auth.log`, `kern.log`).

##### `static unsigned long long GetChannelEventCount(const String& sChannelName)`

Queries total number of log records configured in the specified channel via `EvtOpenLog` / `EvtLogNumberOfLogRecords`.

##### `static EtwEventLevelCounts GetChannelEventLevelCounts(const String& sChannelName)`

Queries event level breakdown counts (Critical, Error, Warning, Information, Verbose) for the specified channel using targeted XPath level queries (`*[System[Level=N]]`).

##### `static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents = 0, int iStartIndex = 0, bool bReverseDirection = true)`

Reads recorded events from the specified channel with pagination offset (`iStartIndex`), limit (`iMaxEvents`), and direction control (`bReverseDirection`). On Windows, uses native `EvtQuery`, `EvtSeek`, `EvtRender`, and `EvtFormatMessage`. When `EvtFormatMessage` returns empty (e.g., unregistered provider resources), `sMessage` automatically falls back to `sRawXml`.

##### `void StartListening(const String& sChannelName, std::function<void(const EtwEvent&)> fnCallback)`

Attaches a live subscription listener callback to receive real-time events published to the target channel.

##### `void StopListening()`

Stops the live event listener subscription.

##### `bool IsListening() const`

Returns `true` if active live subscription is listening.

---

## Code Example

The following sample demonstrates enumerating channels, reading recorded events, and setting up a live event subscription listener.

```cpp
#include "System/Console.h"
#include "System/Diagnostics/EtwLogReader.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    // 1. Enumerate channels
    auto lstChannels = EtwLogReader::GetEventChannels();
    Console::WriteLine("Channels found: " + Convert::ToString(lstChannels.GetCount()));

    // 2. Read events from first channel
    if (lstChannels.GetCount() > 0) {
        String sTarget = lstChannels[0];
        auto lstEvents = EtwLogReader::ReadEvents(sTarget, 10);
        Console::WriteLine("Read " + Convert::ToString(lstEvents.GetCount()) + " events from " + sTarget);
    }

    // 3. Live Subscription Listening
    EtwLogReader reader;
    reader.StartListening("System", [](const EtwEvent& evt) {
        Console::WriteLine("[Live Event ID " + Convert::ToString(evt.iEventId) + "] " + evt.sMessage);
    });

    // ... listen for events ...

    reader.StopListening();
    return 0;
}
```
