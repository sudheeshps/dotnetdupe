# Interlocked

Provides atomic operations for variables that are shared by multiple threads.

## Static Methods

### `static int Increment(int& location)`
Increments a specified variable and stores the result, as an atomic operation.

### `static int Decrement(int& location)`
Decrements a specified variable and stores the result, as an atomic operation.

### `static int Add(int& location1, int value)`
Adds two integers and replaces the first integer with the sum, as an atomic operation.

### `static int Exchange(int& location1, int value)`
Sets a variable to a specified value as an atomic operation and returns the original value.

### `static int CompareExchange(int& location1, int value, int comparand)`
Compares two integers for equality and, if they are equal, replaces the first integer.

## Example Usage

```cpp
using namespace DotNetDupe::System::Threading;

int counter = 0;
Interlocked::Increment(counter);
```
