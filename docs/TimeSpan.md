### class `TimeSpan`

Represents a time interval.

#### Methods

##### `TimeSpan(int64_t ticks)`

Initializes a new instance of the `TimeSpan` class to a specified number of ticks.

**Usage:**
```cpp
TimeSpan ts(10000000); // 1 second
```

##### `static TimeSpan FromDays(double value)`

Returns a `TimeSpan` that represents a specified number of days.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromDays(1);
```

##### `static TimeSpan FromHours(double value)`

Returns a `TimeSpan` that represents a specified number of hours.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromHours(2.5);
```

##### `static TimeSpan FromMinutes(double value)`

Returns a `TimeSpan` that represents a specified number of minutes.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromMinutes(30);
```

##### `static TimeSpan FromSeconds(double value)`

Returns a `TimeSpan` that represents a specified number of seconds.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromSeconds(15.5);
```

##### `static TimeSpan FromMilliseconds(double value)`

Returns a `TimeSpan` that represents a specified number of milliseconds.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromMilliseconds(500);
```

##### `double GetTotalDays() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional days.

**Usage:**
```cpp
double days = ts.GetTotalDays();
```

##### `double GetTotalHours() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional hours.

**Usage:**
```cpp
double hours = ts.GetTotalHours();
```

##### `double GetTotalMinutes() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional minutes.

**Usage:**
```cpp
double minutes = ts.GetTotalMinutes();
```

##### `double GetTotalSeconds() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional seconds.

**Usage:**
```cpp
double seconds = ts.GetTotalSeconds();
```

##### `double GetTotalMilliseconds() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional milliseconds.

**Usage:**
```cpp
double ms = ts.GetTotalMilliseconds();
```

##### `int64_t GetTicks() const`

Gets the number of ticks that represent the value of the current `TimeSpan` structure.

**Usage:**
```cpp
int64_t ticks = ts.GetTicks();
```

#### Fields

##### `static constexpr int64_t TicksPerSecond`

Represents the number of ticks in 1 second.

---

## Code Example

The following example demonstrates how to create `TimeSpan` instances representing different durations, perform arithmetic operations, compare intervals, and use `SmartPointer` to manage them.

```cpp
#include "System/Console.h"
#include "System/TimeSpan.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;

int main() {
    // Create TimeSpan using From* methods
    TimeSpan tsDays = TimeSpan::FromDays(1.5);
    TimeSpan tsHours = TimeSpan::FromHours(3);
    TimeSpan tsSeconds = TimeSpan::FromSeconds(45.5);

    Console::WriteLine("tsDays total hours: ");
    Console::WriteLine(tsDays.GetTotalHours());

    Console::WriteLine("tsHours total minutes: ");
    Console::WriteLine(tsHours.GetTotalMinutes());

    // Perform arithmetic operations
    TimeSpan tsSum = tsHours + tsSeconds;
    Console::WriteLine("tsSum total seconds: ");
    Console::WriteLine(tsSum.GetTotalSeconds());

    TimeSpan tsDiff = tsDays - tsHours;
    Console::WriteLine("tsDiff total days: ");
    Console::WriteLine(tsDiff.GetTotalDays());

    // Comparison operators
    if (tsHours > tsSeconds) {
        Console::WriteLine("tsHours is longer than tsSeconds.");
    }

    // Dynamic memory management using SmartPointer
    auto pSpan = SmartPointer<TimeSpan>::New(TimeSpan::TicksPerSecond * 10);
    Console::WriteLine("pSpan total milliseconds: ");
    Console::WriteLine(pSpan->GetTotalMilliseconds());

    return 0;
}
```
