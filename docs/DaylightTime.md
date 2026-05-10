### class `DaylightTime`

Defines the period of daylight saving time.

#### Methods

##### `DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta)`

Initializes a new instance of the `DaylightTime` class with the specified start, end, and time difference information.

**Usage:**
```cpp
DaylightTime dt(start, end, delta);
```

##### `DateTimeOffset GetStart() const`

Gets the object that represents the date and time when the daylight saving period begins.

**Usage:**
```cpp
DateTimeOffset start = dt.GetStart();
```

##### `DateTimeOffset GetEnd() const`

Gets the object that represents the date and time when the daylight saving period ends.

**Usage:**
```cpp
DateTimeOffset end = dt.GetEnd();
```

##### `TimeSpan GetDelta() const`

Gets the time interval that represents the difference between standard time and daylight saving time.

**Usage:**
```cpp
TimeSpan delta = dt.GetDelta();
```
