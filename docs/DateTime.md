# System::DateTime

The `DateTime` class provides a comprehensive representation of dates and times, heavily inspired by the .NET framework's `System.DateTime`. It stores the time as ticks (1 tick = 100 nanoseconds) elapsed since the epoch (0001-01-01 00:00:00).

## Basic Usage

```cpp
#include "System/DateTime.h"
#include <iostream>

using namespace DotNetDupe::System;

int main() {
    // Get the current local time
    DateTime now = DateTime::Now();
    std::cout << "Now: " << now.ToString().c_str() << std::endl;

    // Create a custom date
    DateTime customDate(2025, 1, 1);
    std::cout << "Custom Date: " << customDate.ToString().c_str() << std::endl;

    // Add days
    DateTime future = now.AddDays(7);
    std::cout << "Next week: " << future.ToString().c_str() << std::endl;

    return 0;
}
```

## Methods
- `Now()`: Gets the current local time.
- `UtcNow()`: Gets the current UTC time.
- `Today()`: Gets the current date, with the time component set to 00:00:00.
- `AddDays()`, `AddMonths()`, `AddYears()`: Returns a new `DateTime` that adds the specified number of days, months, or years to the value of this instance.
- `ToString()`: Converts the value of the current `DateTime` object to its equivalent string representation.
