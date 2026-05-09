### class `DateTimeOffset`

Represents a point in time, typically expressed as a date and time of day relative to Coordinated Universal Time (UTC).

#### Methods

##### `DateTimeOffset(int64_t ticks)`

Initializes a new instance of the `DateTimeOffset` class using the specified number of ticks.

**Usage:**
```cpp
DateTimeOffset dto(638200000000000000);
```

##### `int64_t GetTicks() const`

Gets the number of ticks that represent the date and time of the current `DateTimeOffset` object.

**Usage:**
```cpp
int64_t ticks = dto.GetTicks();
```

##### `TimeSpan operator-(const DateTimeOffset& other) const`

Subtracts a `DateTimeOffset` from another `DateTimeOffset` and returns a `TimeSpan`.

**Usage:**
```cpp
TimeSpan elapsed = dto2 - dto1;
```

##### `bool operator==(const DateTimeOffset& other) const`

Determines whether two specified `DateTimeOffset` objects represent the same point in time.

**Usage:**
```cpp
if (dto1 == dto2) { /* ... */ }
```
