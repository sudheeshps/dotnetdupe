# DateTime

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/DateTime.h"`

Represents an instant in time, typically expressed as a date and time of day (100-nanosecond ticks since `0001-01-01 00:00:00.0000000`).

---

## Syntax

```cpp
enum class DateTimeKind {
    Unspecified = 0,
    Utc = 1,
    Local = 2
};

class DateTime : public Object;
```

---

## Constructors

### `DateTime()`
Initializes a new `DateTime` instance set to 0 ticks (`0001-01-01 00:00:00`).

### `DateTime(int64_t nTicks, DateTimeKind kind = DateTimeKind::Unspecified)`
Initializes a new `DateTime` instance with specified 100-nanosecond ticks and time kind.

### `DateTime(int year, int month, int day)`
Initializes a `DateTime` to a specified year, month, and day with time set to midnight (`00:00:00`).

### `DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind = DateTimeKind::Unspecified)`
Initializes a `DateTime` to a specified date and time.

### `DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind = DateTimeKind::Unspecified)`
Initializes a `DateTime` to a specified date, time, and millisecond precision.

---

## Static Methods

### `static DateTime Now()`
Gets a `DateTime` object that is set to the current date and time on this computer, expressed as local time.

```cpp
DateTime dtNow = DateTime::Now();
```

### `static DateTime UtcNow()`
Gets a `DateTime` object that is set to the current date and time on this computer, expressed as Coordinated Universal Time (UTC).

### `static DateTime Today()`
Gets the current date with the time component set to 00:00:00.

### `static DateTime Parse(const String& s)`
Converts the string representation of a date and time to its `DateTime` equivalent.
- **Parameters:**
  - `s` (`const String&`): A string that contains a date and time to convert.
- **Returns:**
  - `DateTime`: An object that is equivalent to the date and time contained in `s`.
- **Throws:**
  - `FormatException`: If `s` does not contain a valid string representation of a date and time.

### `static DateTime ParseExact(const String& s, const String& format)`
Converts the specified string representation of a date and time to its `DateTime` equivalent using the specified format.
- **Parameters:**
  - `s` (`const String&`): A string containing a date and time to convert.
  - `format` (`const String&`): A format specifier that defines the expected format of `s` (e.g. `"yyyy-MM-dd HH:mm:ss"`).
- **Throws:**
  - `FormatException`: If `s` or `format` is empty, or `s` does not conform to the pattern specified in `format`.

```cpp
DateTime dt = DateTime::ParseExact("2026-08-16 15:45:00", "yyyy-MM-dd HH:mm:ss");
```

### `static bool TryParse(const String& s, DateTime& result)`
Converts the specified string representation of a date and time to its `DateTime` equivalent and returns a value that indicates whether the conversion succeeded.
- **Parameters:**
  - `s` (`const String&`): A string containing a date and time to convert.
  - `result` (`DateTime&`): When this method returns, contains the `DateTime` value equivalent to the date and time contained in `s`, if the conversion succeeded.
- **Returns:**
  - `bool`: `true` if `s` was converted successfully; otherwise, `false`.

```cpp
DateTime parsedDate;
if (DateTime::TryParse("2026-12-25", parsedDate)) {
    Console::WriteLine("Parsed successfully: {0}", parsedDate.ToString());
}
```

### `static bool TryParseExact(const String& s, const String& format, DateTime& result)`
Converts the specified string representation of a date and time to its `DateTime` equivalent using the specified format. Returns `true` if conversion succeeded; otherwise, `false`.

### `static int DaysInMonth(int year, int month)`
Returns the number of days in the specified month and year.

```cpp
int days = DateTime::DaysInMonth(2024, 2); // 29 (leap year)
```

### `static bool IsLeapYear(int year)`
Checks whether the specified year is a leap year.

```cpp
bool bLeap = DateTime::IsLeapYear(2024); // true
```

---

## Properties & Component Accessors

### `int64_t GetTicks() const`
Gets the number of 100-nanosecond ticks that represent the date and time of this instance.

### `DateTimeKind GetKind() const`
Gets a value that indicates whether the time represented by this instance is based on local time, UTC, or neither.

### `int GetYear() const`
Gets the year component of the date represented by this instance.

### `int GetMonth() const`
Gets the month component of the date represented by this instance (`1` through `12`).

### `int GetDay() const`
Gets the day of the month represented by this instance (`1` through `31`).

### `int GetHour() const`
Gets the hour component of the date represented by this instance (`0` through `23`).

### `int GetMinute() const`
Gets the minute component of the date represented by this instance (`0` through `59`).

### `int GetSecond() const`
Gets the second component of the date represented by this instance (`0` through `59`).

### `int GetMillisecond() const`
Gets the milliseconds component of the date represented by this instance (`0` through `999`).

### `int GetDayOfYear() const`
Gets the day of the year represented by this instance (`1` through `366`).

### `int GetDayOfWeek() const`
Gets the day of the week represented by this instance (`0` = Sunday, `1` = Monday, ..., `6` = Saturday).

### `DateTime GetDate() const`
Gets the date component of this instance with the time set to `00:00:00`.

```cpp
DateTime justDate = dtNow.GetDate();
```

### `TimeSpan GetTimeOfDay() const`
Gets the time of day for this instance represented as a `TimeSpan`.

```cpp
TimeSpan timePart = dtNow.GetTimeOfDay();
```

---

## Arithmetic & Transformations

### `DateTime Add(TimeSpan value) const`
Returns a new `DateTime` that adds the value of the specified `TimeSpan` to the value of this instance.

### `DateTime AddDays(double value) const`
Returns a new `DateTime` that adds the specified number of days to the value of this instance.

### `DateTime AddHours(double value) const`
Returns a new `DateTime` that adds the specified number of hours.

### `DateTime AddMinutes(double value) const`
Returns a new `DateTime` that adds the specified number of minutes.

### `DateTime AddSeconds(double value) const`
Returns a new `DateTime` that adds the specified number of seconds.

### `DateTime AddMilliseconds(double value) const`
Returns a new `DateTime` that adds the specified number of milliseconds.

### `DateTime AddMonths(int months) const`
Returns a new `DateTime` that adds the specified number of months.

### `DateTime AddYears(int value) const`
Returns a new `DateTime` that adds the specified number of years.

### `DateTime AddTicks(int64_t value) const`
Returns a new `DateTime` that adds the specified number of ticks.

### `DateTime ToLocalTime() const`
Converts the value of the current `DateTime` object to local time.

### `DateTime ToUniversalTime() const`
Converts the value of the current `DateTime` object to Coordinated Universal Time (UTC).

---

## Formatting & String Representations

### `String ToString() const`
Converts the value of the current `DateTime` object to its equivalent string representation using standard ISO-like formatting (`"yyyy-MM-dd HH:mm:ss"`).

### `String ToString(const String& sFormat) const`
Converts the value of the current `DateTime` object to its equivalent string representation using the specified format (supports `"o"`, `"O"` for ISO 8601 roundtrip format, or custom patterns).

---

## Operators

- `TimeSpan operator-(const DateTime& other) const`: Returns the time interval between two dates.
- `DateTime operator+(const TimeSpan& t) const`: Adds a time span to a date.
- `DateTime operator-(const TimeSpan& t) const`: Subtracts a time span from a date.
- `bool operator==(const DateTime& other) const` / `!=` / `<` / `<=` / `>` / `>=`: Compares date and time instances.

---

## Example

```cpp
#include "System/Console.h"
#include "System/DateTime.h"
#include "System/TimeSpan.h"

using namespace DotNetDupe::System;

int main() {
    DateTime now = DateTime::Now();
    Console::WriteLine("Current Time: {0}", now.ToString());
    Console::WriteLine("Date Part:    {0}", now.GetDate().ToString());
    Console::WriteLine("Time Part:    {0} hours, {1} mins", 
        now.GetTimeOfDay().GetTotalHours(), 
        now.GetTimeOfDay().GetTotalMinutes());

    // Exact Parsing
    DateTime parsed;
    if (DateTime::TryParseExact("2026-12-31 23:59:59", "yyyy-MM-dd HH:mm:ss", parsed)) {
        Console::WriteLine("Parsed Exact: {0}", parsed.ToString());
        TimeSpan remaining = parsed - now;
        Console::WriteLine("Days until new year: {0}", remaining.GetTotalDays());
    }

    Console::WriteLine("Is 2028 a leap year? {0}", DateTime::IsLeapYear(2028));
    Console::WriteLine("Days in Feb 2028:    {0}", DateTime::DaysInMonth(2028, 2));

    return 0;
}
```
