### class `Random`

Represents a pseudo-random number generator, a device that produces a sequence of numbers that meet certain statistical requirements for randomness.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Random.h"

int main() {
    DotNetDupe::System::Random rand;
    std::cout << "Random number between 0 and 100: " << rand.Next(101) << std::endl;
    return 0;
}
```

#### Methods

##### `Random()`

Initializes a new instance of the `Random` class, using a time-dependent default seed value.

##### `Random(int seed)`

Initializes a new instance of the `Random` class, using the specified seed value.

##### `virtual int Next()`

Returns a non-negative random integer.

##### `virtual int Next(int maxValue)`

Returns a non-negative random integer that is less than the specified maximum.

##### `virtual int Next(int minValue, int maxValue)`

Returns a random integer that is within a specified range.

##### `virtual void NextBytes(unsigned char* buffer, int bufferSize)`

Fills the elements of a specified array of bytes with random numbers.

##### `virtual double NextDouble()`

Returns a random floating-point number that is greater than or equal to 0.0, and less than 1.0.
