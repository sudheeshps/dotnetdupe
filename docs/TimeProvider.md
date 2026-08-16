# TimeProvider

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/TimeProvider.h"`

Provides a testable abstraction for time, timestamp generation, and elapsed duration measurements (equivalent to .NET 8 `TimeProvider`).

---

## Syntax

```cpp
class TimeProvider : public Object;
using TimeProviderPtr = SmartPointer<TimeProvider>;
```

---

## Static Methods

### `static TimeProviderPtr GetSystem()`
Gets a `TimeProvider` instance that provides a default implementation based on the system clock.

```cpp
TimeProviderPtr spTime = TimeProvider::GetSystem();
```

---

## Virtual Member Functions

### `virtual DateTimeOffset GetUtcNow() const = 0`
Gets the current Coordinated Universal Time (UTC) date and time.

### `virtual DateTimeOffset GetLocalNow() const = 0`
Gets the current local date and time.

### `virtual int64_t GetTimestamp() const = 0`
Gets the high-frequency counter timestamp.

### `virtual int64_t GetTimestampFrequency() const = 0`
Gets the frequency of the high-frequency counter in ticks per second.

### `TimeSpan GetElapsedTime(int64_t startingTimestamp) const`
Gets the elapsed time interval since `startingTimestamp`.

---

## Example

```cpp
#include "System/Console.h"
#include "System/TimeProvider.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    TimeProviderPtr spTime = TimeProvider::GetSystem();

    int64_t start = spTime->GetTimestamp();
    Thread::Sleep(50);
    TimeSpan elapsed = spTime->GetElapsedTime(start);

    Console::WriteLine("Elapsed Milliseconds: {0}", elapsed.GetTotalMilliseconds());
    return 0;
}
```
