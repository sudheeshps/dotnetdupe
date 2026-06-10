### class `TimeZone`

Represents a time zone.

#### Methods

##### `static TimeZone* GetCurrentTimeZone()`

Gets the time zone of the current computer.

**Usage:**
```cpp
TimeZone* tz = TimeZone::GetCurrentTimeZone();
```

##### `virtual String GetStandardName() const = 0`

Gets the standard time zone name.

**Usage:**
```cpp
String name = tz->GetStandardName();
```

##### `virtual String GetDaylightName() const = 0`

Gets the daylight saving time zone name.

**Usage:**
```cpp
String name = tz->GetDaylightName();
```

##### `virtual TimeSpan GetUtcOffset(const DateTimeOffset& time) = 0`

Returns the Coordinated Universal Time (UTC) offset for the specified point in time.

**Usage:**
```cpp
TimeSpan offset = tz->GetUtcOffset(DateTimeOffset::Now());
```

##### `virtual DaylightTime GetDaylightChanges(int year) = 0`

Returns the daylight saving time period for a particular year.

**Usage:**
```cpp
DaylightTime dt = tz->GetDaylightChanges(2024);
```

##### `virtual bool IsDaylightSavingTime(const DateTimeOffset& time)`

Returns a value indicating whether the specified date and time is within a daylight saving time period.

**Usage:**
```cpp
bool isDst = tz->IsDaylightSavingTime(time);
```

##### `virtual DateTimeOffset ToLocalTime(const DateTimeOffset& time)`

Returns the local time that corresponds to a specified date and time value.

**Usage:**
```cpp
DateTimeOffset local = tz->ToLocalTime(utcTime);
```

##### `virtual DateTimeOffset ToUniversalTime(const DateTimeOffset& time)`

Returns the Coordinated Universal Time (UTC) that corresponds to a specified time.

**Usage:**
```cpp
DateTimeOffset utc = tz->ToUniversalTime(localTime);
```

##### `static bool IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes)`

Returns a value indicating whether the specified date and time is within the specified daylight saving time period.

**Usage:**
```cpp
bool isDst = TimeZone::IsDaylightSavingTime(time, daylightTimes);
```

---

## Code Example

The following example demonstrates how to retrieve the current local `TimeZone`, query its standard and daylight names, determine if a specific time is under daylight saving time, and calculate UTC offsets. It also shows using `SmartPointer` for dynamic allocation.

```cpp
#include "System/Console.h"
#include "System/TimeZone.h"
#include "System/DateTimeOffset.h"
#include "System/DaylightTime.h"
#include "System/TimeSpan.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;

int main() {
    // Get the current local time zone (raw pointer managed by the library)
    TimeZone* tz = TimeZone::GetCurrentTimeZone();

    Console::Write("Standard Name: ");
    Console::WriteLine(tz->GetStandardName());

    Console::Write("Daylight Name: ");
    Console::WriteLine(tz->GetDaylightName());

    // Current point in time
    DateTimeOffset time(638200000000000000); 

    // Check if it is Daylight Saving Time
    bool isDst = tz->IsDaylightSavingTime(time);
    Console::Write("Is Daylight Saving Time: ");
    Console::WriteLine(isDst);

    // Get UTC offset
    TimeSpan offset = tz->GetUtcOffset(time);
    Console::Write("UTC Offset (Hours): ");
    Console::WriteLine(offset.GetTotalHours());

    // Convert to local time
    DateTimeOffset localTime = tz->ToLocalTime(time);
    Console::Write("Local Time Ticks: ");
    Console::WriteLine(localTime.GetTicks());

    // Get daylight saving time details for the year 2026
    DaylightTime dt = tz->GetDaylightChanges(2026);

    // Dynamically manage a copy using SmartPointer
    auto pDaylight = SmartPointer<DaylightTime>::New(dt.GetStart(), dt.GetEnd(), dt.GetDelta());
    Console::Write("Daylight Delta (Minutes): ");
    Console::WriteLine(pDaylight->GetDelta().GetTotalMinutes());

    return 0;
}
```


