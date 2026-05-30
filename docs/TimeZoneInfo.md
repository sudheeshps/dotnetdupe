### class `TimeZoneInfo`

Represents any time zone in the world.

#### Methods

##### `static TimeZoneInfo Local()`

Gets a `TimeZoneInfo` object that represents the local time zone.

**Usage:**
```cpp
TimeZoneInfo local = TimeZoneInfo::Local();
```

##### `static TimeZoneInfo Utc()`

Gets a `TimeZoneInfo` object that represents the Coordinated Universal Time (UTC) zone.

**Usage:**
```cpp
TimeZoneInfo utc = TimeZoneInfo::Utc();
```

##### `String GetId() const`

Gets the time zone identifier.

**Usage:**
```cpp
String id = tzInfo.GetId();
```

##### `String GetDisplayName() const`

Gets the general display name that represents the time zone.

**Usage:**
```cpp
String name = tzInfo.GetDisplayName();
```

##### `String GetStandardName() const`

Gets the display name for the time zone's standard time.

**Usage:**
```cpp
String name = tzInfo.GetStandardName();
```

##### `String GetDaylightName() const`

Gets the display name for the time zone's daylight saving time.

**Usage:**
```cpp
String name = tzInfo.GetDaylightName();
```

##### `TimeSpan GetBaseUtcOffset() const`

Gets the time difference between the current time zone's standard time and Coordinated Universal Time (UTC).

**Usage:**
```cpp
TimeSpan offset = tzInfo.GetBaseUtcOffset();
```

##### `bool SupportsDaylightSavingTime() const`

Gets a value indicating whether the time zone has any daylight saving time rules.

**Usage:**
```cpp
bool supportsDst = tzInfo.SupportsDaylightSavingTime();
```

##### `TimeSpan GetUtcOffset(const DateTimeOffset& dateTimeOffset) const`

Calculates the offset from Coordinated Universal Time (UTC) for a specific date and time.

**Usage:**
```cpp
TimeSpan offset = tzInfo.GetUtcOffset(time);
```

##### `bool IsDaylightSavingTime(const DateTimeOffset& dateTimeOffset) const`

Indicates whether a specified date and time falls within a daylight saving time period.

**Usage:**
```cpp
bool isDst = tzInfo.IsDaylightSavingTime(time);
```

##### `static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone)`

Converts a time to the time in a particular time zone.

**Usage:**
```cpp
DateTimeOffset converted = TimeZoneInfo::ConvertTime(time, destinationTz);
```

##### `static TimeZoneInfo FindSystemTimeZoneById(const String& id)`

Retrieves a `TimeZoneInfo` object from the system registry based on its identifier.

**Usage:**
```cpp
TimeZoneInfo tz = TimeZoneInfo::FindSystemTimeZoneById("UTC");
```

##### `bool Equals(const TimeZoneInfo& other) const`

Determines whether two `TimeZoneInfo` objects are equal.

**Usage:**
```cpp
if (tz1.Equals(tz2)) { /* ... */ }
```

