# Stopwatch

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/Stopwatch.h"`

Provides a high-resolution performance timer mechanism for measuring elapsed time accurately. Supports mocking via `TimeProvider`.

---

## Syntax

```cpp
class Stopwatch : public Object;
```

---

## Static Fields & Constants

- `static const long long Frequency`: Gets the frequency of the timer as the number of ticks per second.
- `static const bool IsHighResolution`: Indicates whether the timer is based on a high-resolution performance counter.

---

## Constructors

### `Stopwatch()`
Initializes a new instance of the `Stopwatch` class using the system high-resolution clock.

### `Stopwatch(const SmartPointer<TimeProvider>& timeProvider)`
Initializes a new instance of the `Stopwatch` class using a testable `TimeProvider` instance.

---

## Member Functions

### `void Start()`
Starts, or resumes, measuring elapsed time for an interval.

### `void Stop()`
Stops measuring elapsed time for an interval.

### `void Reset()`
Stops time interval measurement and resets the elapsed time to zero.

### `void Restart()`
Stops time interval measurement, resets the elapsed time to zero, and starts measuring elapsed time.

### `bool IsRunning() const`
Gets a value indicating whether the `Stopwatch` timer is running.

### `TimeSpan Elapsed() const`
Gets the total elapsed time measured by the current instance as a `TimeSpan`.

### `long long ElapsedMilliseconds() const`
Gets the total elapsed time measured by the current instance, in milliseconds.

### `long long ElapsedTicks() const`
Gets the total elapsed time measured by the current instance, in timer ticks.

---

## Static Methods

### `static Stopwatch StartNew()`
Initializes a new `Stopwatch` instance, sets the elapsed time property to zero, and starts measuring elapsed time.

### `static Stopwatch StartNew(const SmartPointer<TimeProvider>& timeProvider)`
Initializes and starts a new `Stopwatch` using a custom `TimeProvider`.

### `static long long GetTimestamp()`
Gets the current number of ticks in the timer mechanism.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/Stopwatch.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

int main() {
    Stopwatch sw = Stopwatch::StartNew();

    // Perform operations
    Thread::Sleep(120);

    sw.Stop();

    Console::WriteLine("Elapsed Milliseconds: {0} ms", sw.ElapsedMilliseconds());
    Console::WriteLine("Elapsed Ticks:        {0} ticks", sw.ElapsedTicks());
    Console::WriteLine("Total Seconds:        {0:F3} s", sw.Elapsed().GetTotalSeconds());

    return 0;
}
```
