# CriticalSection

A high-performance user-mode synchronization primitive using Win32 `CRITICAL_SECTION`.

## Methods

### `CriticalSection()`
Initializes a new instance of the `CriticalSection` class with a spin count for optimized performance.

### `void Enter()`
Enters the critical section. Blocks if another thread has entered.

### `void Leave()`
Leaves the critical section.

### `bool TryEnter()`
Attempts to enter the critical section without blocking.

**Returns:**
- `true` if the thread successfully entered; otherwise, `false`.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

CriticalSection cs;

cs.Enter();
// Thread-safe work
cs.Leave();
```

