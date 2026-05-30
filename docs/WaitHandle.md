# WaitHandle

The `WaitHandle` class is an abstract base class for synchronization objects that wait for exclusive access to shared resources.

## Methods

### `virtual bool WaitOne()`
Blocks the current thread until the current `WaitHandle` receives a signal.

**Returns:**
- `true` if the current instance receives a signal.

### `virtual bool WaitOne(int millisecondsTimeout)`
Blocks the current thread until the current `WaitHandle` receives a signal, using a 32-bit signed integer to specify the time interval.

**Parameters:**
- `millisecondsTimeout`: The number of milliseconds to wait, or -1 to wait indefinitely.

**Returns:**
- `true` if the current instance receives a signal; otherwise, `false`.

**Exceptions:**
- `TimeoutException`: Thrown if the wait operation times out (for derived classes that implement timeout exceptions).

