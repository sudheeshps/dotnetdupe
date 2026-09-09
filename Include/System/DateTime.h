#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include <cstdint>

namespace DotNetDupe {
	namespace System {

		/// \enum DateTimeKind
		/// \brief Specifies whether a DateTime object represents local time, UTC, or is unspecified.
		enum class DateTimeKind {
			Unspecified = 0, ///< The time represented is not specified as either local time or UTC.
			Utc = 1,         ///< The time represented is UTC.
			Local = 2        ///< The time represented is local time.
		};

		/// \class DateTime
		/// \brief Represents an instant in time, typically expressed as a date and time of day.
		///
		/// \note Conforms to ECMA-335 Partition IV Section 5.16 (System.DateTime), ISO 8601,
		///       and the proleptic Gregorian calendar system. Time values are measured in 100-nanosecond
		///       units called ticks, starting from 00:00:00.0000000 UTC on January 1, 0001 (Common Era).
		class DateTime : public Object {
		public:
			/// \brief Initializes a new instance of DateTime to 0 ticks (0001-01-01 00:00:00) with Unspecified kind.
			DOTNETDUPE_API DateTime() : m_nTicks(0), m_kind(DateTimeKind::Unspecified) { }

			/// \brief Initializes a new instance of DateTime with a specified number of ticks.
			/// \param nTicks A date and time expressed in the number of 100-nanosecond units.
			DOTNETDUPE_API DateTime(int64_t nTicks) : m_nTicks(nTicks), m_kind(DateTimeKind::Unspecified) { }

			/// \brief Initializes a new instance of DateTime with specified ticks and DateTimeKind.
			/// \param nTicks A date and time expressed in ticks.
			/// \param kind One of the enumeration values that indicates whether ticks specifies local time, UTC, or neither.
			DOTNETDUPE_API DateTime(int64_t nTicks, DateTimeKind kind) : m_nTicks(nTicks), m_kind(kind) { }

			/// \brief Initializes a new instance of DateTime to specified year, month, and day.
			/// \param year The year (1 through 9999).
			/// \param month The month (1 through 12).
			/// \param day The day (1 through the number of days in month).
			DOTNETDUPE_API DateTime(int year, int month, int day);

			/// \brief Initializes a new instance of DateTime to specified date and time components.
			/// \param year The year (1 through 9999).
			/// \param month The month (1 through 12).
			/// \param day The day (1 through the number of days in month).
			/// \param hour The hours (0 through 23).
			/// \param minute The minutes (0 through 59).
			/// \param second The seconds (0 through 59).
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second);

			/// \brief Initializes a new instance of DateTime to specified date, time components, and kind.
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);

			/// \brief Initializes a new instance of DateTime with millisecond precision.
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);

			/// \brief Initializes a new instance of DateTime with millisecond precision and kind.
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);

			/// \brief Gets the number of ticks that represent the date and time of this instance.
			/// \return The number of 100-nanosecond ticks.
			int64_t GetTicks() const { return m_nTicks; }

			/// \brief Gets a value that indicates whether the time is local, UTC, or neither.
			/// \return One of the DateTimeKind enumeration values.
			DateTimeKind GetKind() const { return m_kind; }

			/// \brief Gets the year component of the date represented by this instance.
			/// \return The year, between 1 and 9999.
			DOTNETDUPE_API int GetYear() const;

			/// \brief Gets the month component of the date represented by this instance.
			/// \return The month component, expressed as a value between 1 and 12.
			DOTNETDUPE_API int GetMonth() const;

			/// \brief Gets the day of the month represented by this instance.
			/// \return The day component, expressed as a value between 1 and 31.
			DOTNETDUPE_API int GetDay() const;

			/// \brief Gets the hour component of the date represented by this instance.
			/// \return The hour component, expressed as a value between 0 and 23.
			DOTNETDUPE_API int GetHour() const;

			/// \brief Gets the minute component of the date represented by this instance.
			/// \return The minute component, expressed as a value between 0 and 59.
			DOTNETDUPE_API int GetMinute() const;

			/// \brief Gets the seconds component of the date represented by this instance.
			/// \return The seconds component, expressed as a value between 0 and 59.
			DOTNETDUPE_API int GetSecond() const;

			/// \brief Gets the milliseconds component of the date represented by this instance.
			/// \return The milliseconds component, expressed as a value between 0 and 999.
			DOTNETDUPE_API int GetMillisecond() const;

			/// \brief Gets the day of the year represented by this instance.
			/// \return The day of the year, expressed as a value between 1 and 366.
			DOTNETDUPE_API int GetDayOfYear() const;

			/// \brief Gets the day of the week represented by this instance.
			/// \return An integer representing Sunday (0) through Saturday (6).
			DOTNETDUPE_API int GetDayOfWeek() const;

			/// \brief Gets the date component of this instance with the time set to 00:00:00.
			/// \return A new object with the same date as this instance, and the time value set to 12:00:00 midnight (00:00:00).
			DOTNETDUPE_API DateTime GetDate() const;

			/// \brief Gets the time of day for this instance.
			/// \return A time interval that represents the fraction of the day that has elapsed since midnight.
			DOTNETDUPE_API TimeSpan GetTimeOfDay() const;

			/// \brief Returns a new DateTime that adds the value of the specified TimeSpan.
			DOTNETDUPE_API DateTime Add(TimeSpan value) const;

			/// \brief Returns a new DateTime that adds the specified number of days.
			DOTNETDUPE_API DateTime AddDays(double value) const;

			/// \brief Returns a new DateTime that adds the specified number of hours.
			DOTNETDUPE_API DateTime AddHours(double value) const;

			/// \brief Returns a new DateTime that adds the specified number of milliseconds.
			DOTNETDUPE_API DateTime AddMilliseconds(double value) const;

			/// \brief Returns a new DateTime that adds the specified number of minutes.
			DOTNETDUPE_API DateTime AddMinutes(double value) const;

			/// \brief Returns a new DateTime that adds the specified number of months.
			DOTNETDUPE_API DateTime AddMonths(int months) const;

			/// \brief Returns a new DateTime that adds the specified number of seconds.
			DOTNETDUPE_API DateTime AddSeconds(double value) const;

			/// \brief Returns a new DateTime that adds the specified number of ticks.
			DOTNETDUPE_API DateTime AddTicks(int64_t value) const;

			/// \brief Returns a new DateTime that adds the specified number of years.
			DOTNETDUPE_API DateTime AddYears(int value) const;

			/// \brief Gets a DateTime object set to the current date and time on this computer, expressed as local time.
			DOTNETDUPE_API static DateTime Now();

			/// \brief Gets a DateTime object set to the current date and time on this computer, expressed as UTC.
			DOTNETDUPE_API static DateTime UtcNow();

			/// \brief Gets the current date with the time component set to 00:00:00.
			DOTNETDUPE_API static DateTime Today();

			/// \brief Converts the string representation of a date and time to its DateTime equivalent.
			/// \param s A string that contains a date and time to convert.
			/// \return An object that is equivalent to the date and time contained in s.
			/// \throws FormatException s does not contain a valid string representation of a date and time.
			DOTNETDUPE_API static DateTime Parse(const String& s);

			/// \brief Converts the string representation of a date and time to its DateTime equivalent using specified format.
			/// \throws FormatException s does not match format.
			DOTNETDUPE_API static DateTime ParseExact(const String& s, const String& format);

			/// \brief Converts the string representation of a date and time to its DateTime equivalent and returns success flag.
			DOTNETDUPE_API static bool TryParse(const String& s, DateTime& result);

			/// \brief Converts the string representation of a date and time to its DateTime equivalent using format and returns success flag.
			DOTNETDUPE_API static bool TryParseExact(const String& s, const String& format, DateTime& result);

			/// \brief Converts the value of the current DateTime object to local time.
			DOTNETDUPE_API DateTime ToLocalTime() const;

			/// \brief Converts the value of the current DateTime object to Coordinated Universal Time (UTC).
			DOTNETDUPE_API DateTime ToUniversalTime() const;

			/// \brief Converts the value of the current DateTime object to its equivalent string representation.
			DOTNETDUPE_API String ToString() const;

			/// \brief Converts the value of the current DateTime object to its equivalent string representation using format.
			DOTNETDUPE_API String ToString(const String& sFormat) const;

			/// \brief Returns the number of days in the specified month and year.
			DOTNETDUPE_API static int DaysInMonth(int year, int month);

			/// \brief Returns an indication whether the specified year is a leap year.
			DOTNETDUPE_API static bool IsLeapYear(int year);

			/// \brief Subtracts a DateTime from this instance to calculate the elapsed TimeSpan.
			TimeSpan operator-(const DateTime& other) const {
				return TimeSpan(m_nTicks - other.m_nTicks);
			}

			/// \brief Adds a TimeSpan to this instance.
			DateTime operator+(const TimeSpan& t) const {
				return DateTime(m_nTicks + t.GetTicks(), m_kind);
			}

			/// \brief Subtracts a TimeSpan from this instance.
			DateTime operator-(const TimeSpan& t) const {
				return DateTime(m_nTicks - t.GetTicks(), m_kind);
			}

			bool operator==(const DateTime& other) const { return m_nTicks == other.m_nTicks; }
			bool operator!=(const DateTime& other) const { return m_nTicks != other.m_nTicks; }
			bool operator<(const DateTime& other) const { return m_nTicks < other.m_nTicks; }
			bool operator<=(const DateTime& other) const { return m_nTicks <= other.m_nTicks; }
			bool operator>(const DateTime& other) const { return m_nTicks > other.m_nTicks; }
			bool operator>=(const DateTime& other) const { return m_nTicks >= other.m_nTicks; }

		private:
			int64_t m_nTicks;
			DateTimeKind m_kind;
		};
	}
}
