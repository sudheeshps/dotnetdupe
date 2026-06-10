### class `Stopwatch`

Provides a set of methods and properties that you can use to accurately measure elapsed time.

#### Methods

##### `static Stopwatch StartNew()`

Initializes a new `Stopwatch` instance, sets the elapsed time property to zero, and starts measuring elapsed time.

**Usage:**
```cpp
Stopwatch sw = Stopwatch::StartNew();
```

##### `void Start()`

Starts, or resumes, measuring elapsed time for an interval.

**Usage:**
```cpp
Stopwatch sw;
sw.Start();
```

##### `void Stop()`

Stops measuring elapsed time for an interval.

**Usage:**
```cpp
sw.Stop();
```

##### `void Reset()`

Stops time interval measurement and resets the elapsed time to zero.

**Usage:**
```cpp
sw.Reset();
```

##### `void Restart()`

Stops time interval measurement, resets the elapsed time to zero, and starts measuring elapsed time.

**Usage:**
```cpp
sw.Restart();
```

##### `bool IsRunning() const`

Gets a value indicating whether the `Stopwatch` timer is running.

**Usage:**
```cpp
if (sw.IsRunning()) { /* ... */ }
```

##### `TimeSpan Elapsed() const`

Gets the total elapsed time measured by the current instance.

**Usage:**
```cpp
TimeSpan ts = sw.Elapsed();
```

##### `long long ElapsedMilliseconds() const`

Gets the total elapsed time measured by the current instance, in milliseconds.

**Usage:**
```cpp
long long ms = sw.ElapsedMilliseconds();
```

##### `long long ElapsedTicks() const`

Gets the total elapsed time measured by the current instance, in timer ticks.

**Usage:**
```cpp
long long ticks = sw.ElapsedTicks();
```

##### `static long long GetTimestamp()`

Gets the current number of ticks in the timer mechanism.

**Usage:**
```cpp
long long start = Stopwatch::GetTimestamp();
```

#### Fields

##### `static const long long Frequency`

Gets the frequency of the timer as the number of ticks per second.

##### `static const bool IsHighResolution`

Indicates whether the timer is based on a high-resolution performance counter.

---

## Code Example

The following example demonstrates how to create a `Stopwatch` using `SmartPointer`, start and stop timing, read elapsed time in different formats, and restart the stopwatch.

```cpp
#include "System/Console.h"
#include "System/Diagnostics/Stopwatch.h"
#include "System/SmartPointer.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

int main() {
    // Dynamic memory management using SmartPointer
    auto pStopwatch = SmartPointer<Stopwatch>::New();

    Console::WriteLine("Starting stopwatch...");
    pStopwatch->Start();

    // Sleep to simulate elapsed time
    Thread::Sleep(150);

    pStopwatch->Stop();
    Console::WriteLine("Stopwatch stopped.");

    Console::WriteLine("Elapsed milliseconds: ");
    Console::WriteLine(pStopwatch->ElapsedMilliseconds());

    Console::WriteLine("Elapsed ticks: ");
    Console::WriteLine(pStopwatch->ElapsedTicks());

    // Restart the stopwatch
    pStopwatch->Restart();
    Thread::Sleep(50);
    pStopwatch->Stop();

    Console::WriteLine("Elapsed seconds after restart: ");
    Console::WriteLine(pStopwatch->Elapsed().GetTotalSeconds());

    return 0;
}
```
