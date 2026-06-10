### class `DateTimeOffset`

Represents a point in time, typically expressed as a date and time of day relative to Coordinated Universal Time (UTC).

#### Methods

##### `DateTimeOffset(int64_t ticks)`

Initializes a new instance of the `DateTimeOffset` class using the specified number of ticks.

**Usage:**
```cpp
DateTimeOffset dto(638200000000000000);
```

##### `int64_t GetTicks() const`

Gets the number of ticks that represent the date and time of the current `DateTimeOffset` object.

**Usage:**
```cpp
int64_t ticks = dto.GetTicks();
```

##### `TimeSpan operator-(const DateTimeOffset& other) const`

Subtracts a `DateTimeOffset` from another `DateTimeOffset` and returns a `TimeSpan`.

**Usage:**
```cpp
TimeSpan elapsed = dto2 - dto1;
```

##### `bool operator==(const DateTimeOffset& other) const`

Determines whether two specified `DateTimeOffset` objects represent the same point in time.

**Usage:**
```cpp
if (dto1 == dto2) { /* ... */ }
```

---

## Code Example

The following example demonstrates how to create `DateTimeOffset` instances, compare them, and calculate elapsed time intervals using `TimeSpan`. It also demonstrates managing `DateTimeOffset` objects dynamically using `SmartPointer`.

```cpp
#include "System/Console.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;

int main() {
    // Create DateTimeOffset objects on the stack
    DateTimeOffset dtoStart(638200000000000000);
    DateTimeOffset dtoEnd(638200000010000000); // 1 second later (10,000,000 ticks)

    Console::WriteLine("dtoStart ticks: ");
    Console::WriteLine(dtoStart.GetTicks());

    Console::WriteLine("dtoEnd ticks: ");
    Console::WriteLine(dtoEnd.GetTicks());

    // Calculate elapsed time using subtraction operator
    TimeSpan elapsed = dtoEnd - dtoStart;
    Console::WriteLine("Elapsed seconds: ");
    Console::WriteLine(elapsed.GetTotalSeconds());

    // Compare two points in time
    if (dtoEnd > dtoStart) {
        Console::WriteLine("dtoEnd is after dtoStart.");
    }

    // Dynamic memory management using SmartPointer
    auto pDto = SmartPointer<DateTimeOffset>::New(638200000020000000);
    Console::WriteLine("pDto ticks: ");
    Console::WriteLine(pDto->GetTicks());

    return 0;
}
```
