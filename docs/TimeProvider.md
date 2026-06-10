### class `TimeProvider`

Provides an abstraction for time.

#### Methods

##### `virtual DateTimeOffset GetUtcNow() const = 0`

Gets a `DateTimeOffset` value that is set to the current Coordinated Universal Time (UTC) date and time.

**Usage:**
```cpp
auto provider = TimeProvider::GetSystem();
DateTimeOffset now = provider->GetUtcNow();
```

##### `virtual DateTimeOffset GetLocalNow() const = 0`

Gets a `DateTimeOffset` value that is set to the current local date and time.

**Usage:**
```cpp
DateTimeOffset local = provider->GetLocalNow();
```

##### `virtual int64_t GetTimestamp() const = 0`

Gets the current high-resolution timestamp.

**Usage:**
```cpp
int64_t start = provider->GetTimestamp();
```

##### `virtual int64_t GetTimestampFrequency() const = 0`

Gets the frequency of the timestamp counter.

**Usage:**
```cpp
int64_t freq = provider->GetTimestampFrequency();
```

##### `TimeSpan GetElapsedTime(int64_t startingTimestamp) const`

Gets the elapsed time since the specified starting timestamp.

**Usage:**
```cpp
int64_t start = provider->GetTimestamp();
// ...
TimeSpan elapsed = provider->GetElapsedTime(start);
```

##### `static std::shared_ptr<TimeProvider> GetSystem()`

Gets the system's default `TimeProvider` instance.

**Usage:**
```cpp
auto provider = TimeProvider::GetSystem();
```

---

## Code Example

The following example demonstrates how to retrieve the system `TimeProvider` instance, get the current UTC and local times, measure elapsed time with a high-resolution timestamp, and use `Thread::Sleep()` to simulate delay.

```cpp
#include "System/Console.h"
#include "System/TimeProvider.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"
#include "System/SmartPointer.h"
#include "System/Threading/Thread.h"
#include <memory>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

int main() {
    // Get the system TimeProvider
    std::shared_ptr<TimeProvider> provider = TimeProvider::GetSystem();

    // Retrieve the current UTC and Local times
    DateTimeOffset utcNow = provider->GetUtcNow();
    DateTimeOffset localNow = provider->GetLocalNow();

    Console::WriteLine("UTC Ticks: ");
    Console::WriteLine(utcNow.GetTicks());

    Console::WriteLine("Local Ticks: ");
    Console::WriteLine(localNow.GetTicks());

    // Measure elapsed time using the high-resolution timestamp
    int64_t startTimestamp = provider->GetTimestamp();
    
    // Simulate some work using Thread::Sleep
    Thread::Sleep(100);

    int64_t endTimestamp = provider->GetTimestamp();
    TimeSpan elapsed = provider->GetElapsedTime(startTimestamp, endTimestamp);

    // Dynamic memory management using SmartPointer
    auto pElapsed = SmartPointer<TimeSpan>::New(elapsed);

    Console::WriteLine("Elapsed Milliseconds: ");
    Console::WriteLine(pElapsed->GetTotalMilliseconds());

    return 0;
}
```


