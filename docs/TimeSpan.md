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

## Constructors & Factory Methods

### `TimeSpan()`
Initializes a new instance of `TimeSpan` to zero ticks.

### `TimeSpan(int64_t ticks)` / `static TimeSpan FromTicks(int64_t ticks)`
Initializes a new instance of `TimeSpan` to the specified number of ticks.

### `static TimeSpan FromDays(double value)`
Returns a `TimeSpan` that represents a specified number of days.

```cpp
TimeSpan tsSpan = TimeSpan::FromDays(1.5);
```

### `static TimeSpan FromHours(double value)` / `FromMinutes()` / `FromSeconds()` / `FromMilliseconds()`
Returns a `TimeSpan` that represents the specified time units.

```cpp
TimeSpan tsTimeout = TimeSpan::FromSeconds(30);
```

---

## Properties & Calculations

### `int64_t GetTicks() const`
Gets the number of ticks that represent the value of the current `TimeSpan`.

### `double GetTotalMilliseconds() const` / `GetTotalSeconds()` / `GetTotalMinutes()` / `GetTotalHours()` / `GetTotalDays()`
Gets the value of the current `TimeSpan` structure expressed in whole and fractional time units.

```cpp
double dMinutes = tsTimeout.GetTotalMinutes(); // 0.5
```

### `TimeSpan operator+(const TimeSpan& other) const` / `operator-()`
Adds or subtracts another `TimeSpan`.

### `bool operator==(const TimeSpan& other) const` / `!=` / `<` / `<=` / `>` / `>=`
Relational and equality comparison operators comparing tick durations.

---

## Example

```cpp
#include "System/Console.h"
#include "System/TimeSpan.h"

using namespace DotNetDupe::System;

int main() {
    TimeSpan tsWorkDay = TimeSpan::FromHours(8);
    TimeSpan tsLunch = TimeSpan::FromMinutes(45);

    TimeSpan tsProductiveTime = tsWorkDay - tsLunch;
    Console::WriteLine("Productive Hours: {0}", tsProductiveTime.GetTotalHours());
    Console::WriteLine("Total Minutes: {0}", tsProductiveTime.GetTotalMinutes());

    return 0;
}
```
