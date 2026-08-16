# TimeSpan

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/TimeSpan.h"`

Represents a time interval (duration of time) measured as a positive or negative number of 100-nanosecond ticks.

---

## Syntax

```cpp
class TimeSpan : public Object;
```

---

## Constants

- `TicksPerMillisecond = 10,000`
- `TicksPerSecond = 10,000,000`
- `TicksPerMinute = 600,000,000`
- `TicksPerHour = 36,000,000,000`
- `TicksPerDay = 864,000,000,000`

---

## Factory Methods

### `static TimeSpan FromDays(double value)`
Returns a `TimeSpan` that represents a specified number of days.

```cpp
TimeSpan span = TimeSpan::FromDays(1.5);
```

### `static TimeSpan FromHours(double value)` / `FromMinutes()` / `FromSeconds()` / `FromMilliseconds()`
Returns a `TimeSpan` that represents the specified time units.

```cpp
TimeSpan timeout = TimeSpan::FromSeconds(30);
```

---

## Properties & Calculations

### `int64_t GetTicks() const`
Gets the number of ticks that represent the value of the current `TimeSpan`.

### `double GetTotalMilliseconds() const` / `GetTotalSeconds()` / `GetTotalMinutes()` / `GetTotalHours()` / `GetTotalDays()`
Gets the value of the current `TimeSpan` structure expressed in whole and fractional time units.

```cpp
double minutes = timeout.GetTotalMinutes(); // 0.5
```

### `TimeSpan operator+(const TimeSpan& other) const` / `operator-()`
Adds or subtracts another `TimeSpan`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/TimeSpan.h"

using namespace DotNetDupe::System;

int main() {
    TimeSpan workDay = TimeSpan::FromHours(8);
    TimeSpan lunch = TimeSpan::FromMinutes(45);

    TimeSpan productiveTime = workDay - lunch;
    Console::WriteLine("Productive Hours: {0}", productiveTime.GetTotalHours());
    Console::WriteLine("Total Minutes: {0}", productiveTime.GetTotalMinutes());

    return 0;
}
```
