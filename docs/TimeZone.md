### class `TimeZone`

Represents a time zone.

#### Methods

##### `static TimeZone* GetCurrentTimeZone()`

Gets the time zone of the current computer.

**Usage:**
```cpp
TimeZone* tz = TimeZone::GetCurrentTimeZone();
```

##### `virtual String GetStandardName() const = 0`

Gets the standard time zone name.

**Usage:**
```cpp
String name = tz->GetStandardName();
```

##### `virtual String GetDaylightName() const = 0`

Gets the daylight saving time zone name.

**Usage:**
```cpp
String name = tz->GetDaylightName();
```

##### `virtual TimeSpan GetUtcOffset(const DateTimeOffset& time) = 0`

Returns the Coordinated Universal Time (UTC) offset for the specified point in time.

**Usage:**
```cpp
TimeSpan offset = tz->GetUtcOffset(DateTimeOffset::Now());
```

##### `virtual DaylightTime GetDaylightChanges(int year) = 0`

Returns the daylight saving time period for a particular year.

**Usage:**
```cpp
DaylightTime dt = tz->GetDaylightChanges(2024);
```

##### `virtual bool IsDaylightSavingTime(const DateTimeOffset& time)`

Returns a value indicating whether the specified date and time is within a daylight saving time period.

**Usage:**
```cpp
bool isDst = tz->IsDaylightSavingTime(time);
```

##### `virtual DateTimeOffset ToLocalTime(const DateTimeOffset& time)`

Returns the local time that corresponds to a specified date and time value.

**Usage:**
```cpp
DateTimeOffset local = tz->ToLocalTime(utcTime);
```

##### `virtual DateTimeOffset ToUniversalTime(const DateTimeOffset& time)`

Returns the Coordinated Universal Time (UTC) that corresponds to a specified time.

**Usage:**
```cpp
DateTimeOffset utc = tz->ToUniversalTime(localTime);
```

##### `static bool IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes)`

Returns a value indicating whether the specified date and time is within the specified daylight saving time period.

**Usage:**
```cpp
bool isDst = TimeZone::IsDaylightSavingTime(time, daylightTimes);
```

