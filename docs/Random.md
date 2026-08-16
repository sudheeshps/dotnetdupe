# Random

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Random.h"`

Represents a pseudo-random number generator, an algorithm that produces a sequence of numbers that meet certain statistical requirements for randomness.

---

## Syntax

```cpp
class Random : public Object;
```

---

## Constructors

### `Random()`
Initializes a new instance of the `Random` class using a time-dependent default seed value.

### `Random(int seed)`
Initializes a new instance of the `Random` class using the specified seed value for reproducible sequences.

```cpp
Random objRng(12345);
```

---

## Member Functions

### `virtual int Next()`
Returns a non-negative random integer.

### `virtual int Next(int maxValue)`
Returns a non-negative random integer that is less than the specified maximum.

- **Throws:**
  - `ArgumentOutOfRangeException`: If `maxValue < 0`.

```cpp
int iRoll = objRng.Next(6) + 1; // 1 to 6
```

### `virtual int Next(int minValue, int maxValue)`
Returns a random integer that is within a specified range (`minValue <= value < maxValue`).

- **Throws:**
  - `ArgumentOutOfRangeException`: If `minValue > maxValue`.

```cpp
int iTemp = objRng.Next(-10, 40);
```

### `virtual double NextDouble()`
Returns a random floating-point number that is greater than or equal to `0.0`, and less than `1.0`.

### `virtual void NextBytes(unsigned char* buffer, int bufferSize)`
Fills the elements of a specified byte buffer with random numbers.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Random.h"

using namespace DotNetDupe::System;

int main() {
    Random objRng;

    Console::WriteLine("Random roll (1-6): {0}", objRng.Next(1, 7));
    Console::WriteLine("Random double:     {0}", objRng.NextDouble());

    return 0;
}
```
