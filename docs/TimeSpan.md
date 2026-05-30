### class `TimeSpan`

Represents a time interval.

#### Methods

##### `TimeSpan(int64_t ticks)`

Initializes a new instance of the `TimeSpan` class to a specified number of ticks.

**Usage:**
```cpp
TimeSpan ts(10000000); // 1 second
```

##### `static TimeSpan FromDays(double value)`

Returns a `TimeSpan` that represents a specified number of days.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromDays(1);
```

##### `static TimeSpan FromHours(double value)`

Returns a `TimeSpan` that represents a specified number of hours.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromHours(2.5);
```

##### `static TimeSpan FromMinutes(double value)`

Returns a `TimeSpan` that represents a specified number of minutes.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromMinutes(30);
```

##### `static TimeSpan FromSeconds(double value)`

Returns a `TimeSpan` that represents a specified number of seconds.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromSeconds(15.5);
```

##### `static TimeSpan FromMilliseconds(double value)`

Returns a `TimeSpan` that represents a specified number of milliseconds.

**Usage:**
```cpp
TimeSpan ts = TimeSpan::FromMilliseconds(500);
```

##### `double GetTotalDays() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional days.

**Usage:**
```cpp
double days = ts.GetTotalDays();
```

##### `double GetTotalHours() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional hours.

**Usage:**
```cpp
double hours = ts.GetTotalHours();
```

##### `double GetTotalMinutes() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional minutes.

**Usage:**
```cpp
double minutes = ts.GetTotalMinutes();
```

##### `double GetTotalSeconds() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional seconds.

**Usage:**
```cpp
double seconds = ts.GetTotalSeconds();
```

##### `double GetTotalMilliseconds() const`

Gets the value of the current `TimeSpan` structure expressed in whole and fractional milliseconds.

**Usage:**
```cpp
double ms = ts.GetTotalMilliseconds();
```

##### `int64_t GetTicks() const`

Gets the number of ticks that represent the value of the current `TimeSpan` structure.

**Usage:**
```cpp
int64_t ticks = ts.GetTicks();
```

#### Fields

##### `static constexpr int64_t TicksPerSecond`

Represents the number of ticks in 1 second.

