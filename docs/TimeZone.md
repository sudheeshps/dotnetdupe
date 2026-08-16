# TimeZone

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/TimeZone.h"`

Represents a time zone. Provides conversions between UTC and local time, daylight saving status, and standard/daylight names.

---

## Syntax

```cpp
class TimeZone : public Object;
```

---

## Static Methods

### `static TimeZone* GetCurrentTimeZone()`
Gets the time zone of the current computer system.

```cpp
TimeZone* pCurrentTz = TimeZone::GetCurrentTimeZone();
```

### `static bool IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes)`
Returns a value indicating whether the specified date and time is within the specified daylight saving time period.

---

## Member Functions

### `virtual String GetStandardName() const = 0`
Gets the standard time zone name.

### `virtual String GetDaylightName() const = 0`
Gets the daylight saving time zone name.

### `virtual TimeSpan GetUtcOffset(const DateTimeOffset& time) = 0`
Returns the Coordinated Universal Time (UTC) offset for the specified local time.

### `virtual bool IsDaylightSavingTime(const DateTimeOffset& time)`
Returns a value indicating whether the specified date and time is within a daylight saving time period.

### `virtual DateTimeOffset ToLocalTime(const DateTimeOffset& time)`
Converts a UTC time value to local time.

### `virtual DateTimeOffset ToUniversalTime(const DateTimeOffset& time)`
Converts a local time value to UTC.

---

## Example

```cpp
#include "System/Console.h"
#include "System/TimeZone.h"
#include "System/DateTimeOffset.h"

using namespace DotNetDupe::System;

int main() {
    TimeZone* pTz = TimeZone::GetCurrentTimeZone();
    if (pTz != nullptr) {
        Console::WriteLine("Standard Name: {0}", pTz->GetStandardName());
        Console::WriteLine("Daylight Name: {0}", pTz->GetDaylightName());

        DateTimeOffset now = DateTimeOffset::Now();
        Console::WriteLine("Is Daylight Saving: {0}", pTz->IsDaylightSavingTime(now));
    }

    return 0;
}
```
