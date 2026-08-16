# DaylightTime

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/DaylightTime.h"`

Defines the period of Daylight Saving Time with start date, end date, and time delta offset.

---

## Syntax

```cpp
class DaylightTime : public Object;
```

---

## Constructors

### `DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta)`
Initializes a new instance of the `DaylightTime` class with specified start and end dates and time change delta.

---

## Properties

### `DateTimeOffset GetStart() const`
Gets the date and time when the daylight saving period begins.

### `DateTimeOffset GetEnd() const`
Gets the date and time when the daylight saving period ends.

### `TimeSpan GetDelta() const`
Gets the time interval that represents the difference between standard time and daylight saving time (typically 1 hour).

---

## Example

```cpp
#include "System/Console.h"
#include "System/DaylightTime.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"

using namespace DotNetDupe::System;

int main() {
    DateTimeOffset start = DateTimeOffset::Now();
    DateTimeOffset end = start;
    TimeSpan delta = TimeSpan::FromHours(1);

    DaylightTime dst(start, end, delta);
    Console::WriteLine("DST Delta: {0} hours", dst.GetDelta().GetTotalHours());

    return 0;
}
```
