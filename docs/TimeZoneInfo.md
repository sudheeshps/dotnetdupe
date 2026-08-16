# TimeZoneInfo

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/TimeZoneInfo.h"`

Represents any time zone in the world with Daylight Saving Time adjustments, conversions, and system time zone lookups.

---

## Syntax

```cpp
class TimeZoneInfo : public Object;
```

---

## Static Properties / Methods

### `static TimeZoneInfo Local()`
Gets a `TimeZoneInfo` object that represents the local time zone.

```cpp
TimeZoneInfo localTz = TimeZoneInfo::Local();
```

### `static TimeZoneInfo Utc()`
Gets a `TimeZoneInfo` object that represents the Coordinated Universal Time (UTC) zone.

### `static TimeZoneInfo FindSystemTimeZoneById(const String& id)`
Retrieves a `TimeZoneInfo` object from the registry or system time zone database by its identifier (e.g. `"Eastern Standard Time"`, `"UTC"`).

- **Throws:**
  - `ArgumentException`: If the time zone identifier cannot be found.

### `static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone)`
Converts a `DateTimeOffset` value to the time in a destination time zone.

---

## Properties & Calculations

### `String GetId() const` / `GetDisplayName()` / `GetStandardName()` / `GetDaylightName()`
Gets the unique identifier, display name, standard name, and daylight name of the time zone.

### `TimeSpan GetBaseUtcOffset() const`
Gets the time difference between the current time zone's standard time and Coordinated Universal Time (UTC).

### `bool SupportsDaylightSavingTime() const`
Gets a value indicating whether the time zone has any daylight saving time rules.

### `TimeSpan GetUtcOffset(const DateTimeOffset& dateTimeOffset) const`
Calculates the offset from UTC for a specific point in time in this time zone.

---

## Example

```cpp
#include "System/Console.h"
#include "System/TimeZoneInfo.h"
#include "System/DateTimeOffset.h"

using namespace DotNetDupe::System;

int main() {
    TimeZoneInfo localTz = TimeZoneInfo::Local();
    Console::WriteLine("Local Zone ID: {0}", localTz.GetId());
    Console::WriteLine("Display Name: {0}", localTz.GetDisplayName());
    Console::WriteLine("Base UTC Offset: {0} hours", localTz.GetBaseUtcOffset().GetTotalHours());

    DateTimeOffset now = DateTimeOffset::Now();
    Console::WriteLine("Is DST: {0}", localTz.IsDaylightSavingTime(now));

    return 0;
}
```
