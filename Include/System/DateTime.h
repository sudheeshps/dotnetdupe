#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include <cstdint>

namespace DotNetDupe {
	namespace System {
		enum class DateTimeKind {
			Unspecified = 0,
			Utc = 1,
			Local = 2
		};

		class DateTime : public Object {
		public:
			DOTNETDUPE_API DateTime() : m_nTicks(0), m_kind(DateTimeKind::Unspecified) { }
			DOTNETDUPE_API DateTime(int64_t nTicks) : m_nTicks(nTicks), m_kind(DateTimeKind::Unspecified) { }
			DOTNETDUPE_API DateTime(int64_t nTicks, DateTimeKind kind) : m_nTicks(nTicks), m_kind(kind) { }
			DOTNETDUPE_API DateTime(int year, int month, int day);
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second);
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
			DOTNETDUPE_API DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);

			int64_t GetTicks() const { return m_nTicks; }
			DateTimeKind GetKind() const { return m_kind; }

			DOTNETDUPE_API int GetYear() const;
			DOTNETDUPE_API int GetMonth() const;
			DOTNETDUPE_API int GetDay() const;
			DOTNETDUPE_API int GetHour() const;
			DOTNETDUPE_API int GetMinute() const;
			DOTNETDUPE_API int GetSecond() const;
			DOTNETDUPE_API int GetMillisecond() const;
			DOTNETDUPE_API int GetDayOfYear() const;
			DOTNETDUPE_API int GetDayOfWeek() const; // 0 = Sunday, 1 = Monday, ... 6 = Saturday

			DOTNETDUPE_API DateTime GetDate() const;
			DOTNETDUPE_API TimeSpan GetTimeOfDay() const;

			DOTNETDUPE_API DateTime Add(TimeSpan value) const;
			DOTNETDUPE_API DateTime AddDays(double value) const;
			DOTNETDUPE_API DateTime AddHours(double value) const;
			DOTNETDUPE_API DateTime AddMilliseconds(double value) const;
			DOTNETDUPE_API DateTime AddMinutes(double value) const;
			DOTNETDUPE_API DateTime AddMonths(int months) const;
			DOTNETDUPE_API DateTime AddSeconds(double value) const;
			DOTNETDUPE_API DateTime AddTicks(int64_t value) const;
			DOTNETDUPE_API DateTime AddYears(int value) const;

			DOTNETDUPE_API static DateTime Now();
			DOTNETDUPE_API static DateTime UtcNow();
			DOTNETDUPE_API static DateTime Today();

			DOTNETDUPE_API static DateTime Parse(const String& s);
			DOTNETDUPE_API static DateTime ParseExact(const String& s, const String& format);
			DOTNETDUPE_API static bool TryParse(const String& s, DateTime& result);
			DOTNETDUPE_API static bool TryParseExact(const String& s, const String& format, DateTime& result);

			DOTNETDUPE_API DateTime ToLocalTime() const;
			DOTNETDUPE_API DateTime ToUniversalTime() const;

			DOTNETDUPE_API String ToString() const;
			DOTNETDUPE_API String ToString(const String& sFormat) const;

			DOTNETDUPE_API static int DaysInMonth(int year, int month);
			DOTNETDUPE_API static bool IsLeapYear(int year);

			TimeSpan operator-(const DateTime& other) const {
				return TimeSpan(m_nTicks - other.m_nTicks);
			}

			DateTime operator+(const TimeSpan& t) const {
				return DateTime(m_nTicks + t.GetTicks(), m_kind);
			}

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
