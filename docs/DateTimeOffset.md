# DateTimeOffset

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/DateTimeOffset.h"`

Represents a point in time, typically expressed as a date and time of day relative to Coordinated Universal Time (UTC).

---

## Syntax

```cpp
class DateTimeOffset : public Object;
```

---

## Constructors

### `DateTimeOffset()`
Initializes a new instance of `DateTimeOffset` set to 0 ticks.

### `DateTimeOffset(int64_t ticks)`
Initializes a new instance of `DateTimeOffset` with the specified number of ticks.

---

## Static Methods

### `static DateTimeOffset Now()`
Gets a `DateTimeOffset` object whose date and time are set to the current local time and whose offset is the local time's offset from UTC.

### `static DateTimeOffset UtcNow()`
Gets a `DateTimeOffset` object whose date and time are set to the current UTC time.

```cpp
DateTimeOffset dto = DateTimeOffset::UtcNow();
```

---

## Member Functions

### `int64_t GetTicks() const`
Gets the number of ticks that represent the date and time of the current `DateTimeOffset` object.

### `String ToString() const` / `String ToString(const String& sFormat) const`
Converts the value of the current `DateTimeOffset` object to its equivalent string representation.

### `TimeSpan operator-(const DateTimeOffset& other) const`
Subtracts one `DateTimeOffset` object from another to yield a time interval (`TimeSpan`).

---

## Example

```cpp
#include "System/Console.h"
#include "System/DateTimeOffset.h"

using namespace DotNetDupe::System;

int main() {
    DateTimeOffset dtoNow = DateTimeOffset::Now();
    Console::WriteLine("Local DateTimeOffset: {0}", dtoNow.ToString());

    DateTimeOffset dtoUtc = DateTimeOffset::UtcNow();
    Console::WriteLine("UTC DateTimeOffset: {0}", dtoUtc.ToString());

    return 0;
}
```
