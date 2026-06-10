### class `Random`

Represents a pseudo-random number generator.

#### Methods

##### `Random()`

Initializes a new instance of the `Random` class, using a time-dependent default seed value.

**Usage:**
```cpp
Random rand;
```

##### `Random(int seed)`

Initializes a new instance of the `Random` class, using the specified seed value.

**Parameters:**
- `seed`: A number used to calculate a starting value for the pseudo-random number sequence.

**Usage:**
```cpp
Random rand(12345);
```

##### `virtual int Next()`

Returns a non-negative random integer.

**Returns:**
- A 32-bit signed integer that is greater than or equal to 0 and less than `INT_MAX`.

**Usage:**
```cpp
int val = rand.Next();
```

##### `virtual int Next(int maxValue)`

Returns a non-negative random integer that is less than the specified maximum.

**Parameters:**
- `maxValue`: The exclusive upper bound of the random number to be generated. `maxValue` must be greater than or equal to 0.

**Returns:**
- A 32-bit signed integer that is greater than or equal to 0, and less than `maxValue`.

**Usage:**
```cpp
int val = rand.Next(100); // 0-99
```

##### `virtual int Next(int minValue, int maxValue)`

Returns a random integer that is within a specified range.

**Parameters:**
- `minValue`: The inclusive lower bound of the random number returned.
- `maxValue`: The exclusive upper bound of the random number returned. `maxValue` must be greater than or equal to `minValue`.

**Returns:**
- A 32-bit signed integer greater than or equal to `minValue` and less than `maxValue`.

**Usage:**
```cpp
int val = rand.Next(10, 20); // 10-19
```

##### `virtual void NextBytes(unsigned char* buffer, int bufferSize)`

Fills the elements of a specified array of bytes with random numbers.

**Parameters:**
- `buffer`: An array of bytes to contain random numbers.
- `bufferSize`: The size of the buffer.

**Usage:**
```cpp
unsigned char buffer[10];
rand.NextBytes(buffer, 10);
```

##### `virtual double NextDouble()`

Returns a random floating-point number that is greater than or equal to 0.0, and less than 1.0.

**Returns:**
- A double-precision floating-point number that is greater than or equal to 0.0, and less than 1.0.

**Usage:**
```cpp
double val = rand.NextDouble();
```

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `Random`.

```cpp
#include "System/Random.h"
#include "System/Console.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;

int main() {
    try {
        // Initialize Random with seed
        Random rand(42);

        // Next positive integer
        int val1 = rand.Next();
        Console::Write("Random Int: ");
        Console::WriteLine(val1);

        // Next integer with max limit (exclusive)
        int val2 = rand.Next(100);
        Console::Write("Random Int (0-99): ");
        Console::WriteLine(val2);

        // Next integer within range (min inclusive, max exclusive)
        int val3 = rand.Next(50, 60);
        Console::Write("Random Int (50-59): ");
        Console::WriteLine(val3);

        // Next double
        double val4 = rand.NextDouble();
        Console::Write("Random Double (0.0-1.0): ");
        Console::WriteLine(val4);

        // Fill buffer with random bytes
        unsigned char byteBuffer[5];
        rand.NextBytes(byteBuffer, 5);
        Console::Write("Random Bytes: ");
        for (int i = 0; i < 5; i++) {
            Console::Write((int)byteBuffer[i]);
            Console::Write(" ");
        }
        Console::WriteLine();

    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


