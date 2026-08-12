#include "pch.h"
#include "System/DateTime.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/ArgumentException.h"
#include "System/FormatException.h"
#include "System/String.h"
#include <chrono>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace DotNetDupe {
	namespace System {

		constexpr int64_t TicksPerMillisecond = 10000;
		constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
		constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
		constexpr int64_t TicksPerHour = TicksPerMinute * 60;
		constexpr int64_t TicksPerDay = TicksPerHour * 24;
		constexpr int64_t MinTicks = 0;
		constexpr int64_t MaxTicks = 3155378975999999999LL;
		constexpr int64_t UnixEpochTicks = 621355968000000000LL;

		constexpr int DaysToMonth365 [] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
		constexpr int DaysToMonth366 [] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

		static int64_t DateToTicks(int year, int month, int day) {
			if (year >= 1 && year <= 9999 && month >= 1 && month <= 12) {
				const int* days = DateTime::IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
				if (day >= 1 && day <= days [month] - days [month - 1]) {
					int y = year - 1;
					int n = y * 365 + y / 4 - y / 100 + y / 400 + days [month - 1] + day - 1;
					return (int64_t)n * TicksPerDay;
				}
			}
			throw ArgumentOutOfRangeException("Year, Month, and Day parameters describe an un-representable DateTime.");
		}

		static int64_t TimeToTicks(int hour, int minute, int second) {
			if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
				int64_t totalSeconds = (int64_t)hour * 3600 + (int64_t)minute * 60 + (int64_t)second;
				return totalSeconds * TicksPerSecond;
			}
			throw ArgumentOutOfRangeException("Hour, Minute, and Second parameters describe an un-representable DateTime.");
		}

		static void GetDatePart(int64_t ticks, int& year, int& month, int& day) {
			int n = (int)(ticks / TicksPerDay);
			int y400 = n / 146097; n -= y400 * 146097;
			int y100 = n / 36524; if (y100 == 4) y100 = 3; n -= y100 * 36524;
			int y4 = n / 1461; n -= y4 * 1461;
			int y1 = n / 365; if (y1 == 4) y1 = 3; n -= y1 * 365;
			year = y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
			const int* days = DateTime::IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			month = 1;
			while (n >= days [month]) month++;
			day = n - days [month - 1] + 1;
		}

		DateTime::DateTime(int year, int month, int day)
			: m_nTicks(DateToTicks(year, month, day)), m_kind(DateTimeKind::Unspecified) { }

		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
			: m_nTicks(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second)), m_kind(DateTimeKind::Unspecified) { }

		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
			: m_nTicks(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second)), m_kind(kind) { }

		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
			: m_nTicks(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + millisecond * TicksPerMillisecond), m_kind(DateTimeKind::Unspecified) {
			if (millisecond < 0 || millisecond >= 1000)
				throw ArgumentOutOfRangeException("Millisecond must be between 0 and 999.");
		}

		DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
			: m_nTicks(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + millisecond * TicksPerMillisecond), m_kind(kind) {
			if (millisecond < 0 || millisecond >= 1000)
				throw ArgumentOutOfRangeException("Millisecond must be between 0 and 999.");
		}

		int DateTime::GetYear() const { int y, m, d; GetDatePart(m_nTicks, y, m, d); return y; }
		int DateTime::GetMonth() const { int y, m, d; GetDatePart(m_nTicks, y, m, d); return m; }
		int DateTime::GetDay() const { int y, m, d; GetDatePart(m_nTicks, y, m, d); return d; }
		int DateTime::GetHour() const { return (int)((m_nTicks / TicksPerHour) % 24); }
		int DateTime::GetMinute() const { return (int)((m_nTicks / TicksPerMinute) % 60); }
		int DateTime::GetSecond() const { return (int)((m_nTicks / TicksPerSecond) % 60); }
		int DateTime::GetMillisecond() const { return (int)((m_nTicks / TicksPerMillisecond) % 1000); }

		int DateTime::GetDayOfYear() const {
			int y, m, d;
			GetDatePart(m_nTicks, y, m, d);
			const int* days = IsLeapYear(y) ? DaysToMonth366 : DaysToMonth365;
			return days [m - 1] + d;
		}

		int DateTime::GetDayOfWeek() const {
			return (int)((m_nTicks / TicksPerDay + 1) % 7);
		}

		DateTime DateTime::GetDate() const {
			return DateTime(m_nTicks - (m_nTicks % TicksPerDay), m_kind);
		}

		TimeSpan DateTime::GetTimeOfDay() const {
			return TimeSpan(m_nTicks % TicksPerDay);
		}

		DateTime DateTime::Add(TimeSpan value) const {
			return AddTicks(value.GetTicks());
		}

		DateTime DateTime::AddDays(double value) const {
			return AddMilliseconds(value * 86400000.0);
		}

		DateTime DateTime::AddHours(double value) const {
			return AddMilliseconds(value * 3600000.0);
		}

		DateTime DateTime::AddMilliseconds(double value) const {
			int64_t ticks = (int64_t)(value * TicksPerMillisecond + (value >= 0.0 ? 0.5 : -0.5));
			return AddTicks(ticks);
		}

		DateTime DateTime::AddMinutes(double value) const {
			return AddMilliseconds(value * 60000.0);
		}

		DateTime DateTime::AddMonths(int months) const {
			if (months < -120000 || months > 120000) throw ArgumentOutOfRangeException("Months value is out of range.");
			int y, m, d; GetDatePart(m_nTicks, y, m, d);
			int i = m - 1 + months;
			if (i >= 0) { m = i % 12 + 1; y = y + i / 12; }
			else { m = 12 + (i + 1) % 12; y = y + (i - 11) / 12; }
			if (y < 1 || y > 9999) throw ArgumentOutOfRangeException("DateTime result is out of range.");
			int days = DaysInMonth(y, m);
			if (d > days) d = days;
			return DateTime((int64_t)(DateToTicks(y, m, d) + m_nTicks % TicksPerDay), m_kind);
		}

		DateTime DateTime::AddSeconds(double value) const {
			return AddMilliseconds(value * 1000.0);
		}

		DateTime DateTime::AddTicks(int64_t value) const {
			int64_t ticks = m_nTicks + value;
			if (ticks < MinTicks || ticks > MaxTicks) throw ArgumentOutOfRangeException("DateTime result is out of range.");
			return DateTime(ticks, m_kind);
		}

		DateTime DateTime::AddYears(int value) const {
			if (value < -10000 || value > 10000) throw ArgumentOutOfRangeException("Years value is out of range.");
			return AddMonths(value * 12);
		}

		DateTime DateTime::Now() {
			DateTime utc = UtcNow();
			return utc.ToLocalTime();
		}

		DateTime DateTime::UtcNow() {
			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();
			int64_t micro = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
			int64_t ticks = (micro * 10) + UnixEpochTicks;
			return DateTime(ticks, DateTimeKind::Utc);
		}

		DateTime DateTime::Today() {
			return Now().GetDate();
		}

		DateTime DateTime::ToLocalTime() const {
			if (m_kind == DateTimeKind::Local) return *this;
			int64_t time = (m_nTicks - UnixEpochTicks) / TicksPerSecond;
			struct tm t;
			_localtime64_s(&t, &time);
			int year = t.tm_year + 1900;
			int month = t.tm_mon + 1;
			int day = t.tm_mday;
			int hour = t.tm_hour;
			int minute = t.tm_min;
			int second = t.tm_sec;
			int64_t ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + m_nTicks % TicksPerSecond;
			return DateTime(ticks, DateTimeKind::Local);
		}

		DateTime DateTime::ToUniversalTime() const {
			if (m_kind == DateTimeKind::Utc) return *this;
			int64_t time = (m_nTicks - UnixEpochTicks) / TicksPerSecond;
			struct tm t;
			_gmtime64_s(&t, &time);
			int year = t.tm_year + 1900;
			int month = t.tm_mon + 1;
			int day = t.tm_mday;
			int hour = t.tm_hour;
			int minute = t.tm_min;
			int second = t.tm_sec;
			int64_t ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + m_nTicks % TicksPerSecond;
			return DateTime(ticks, DateTimeKind::Utc);
		}

		String DateTime::ToString() const {
			return ToString("yyyy-MM-dd HH:mm:ss");
		}

		String DateTime::ToString(const String& sFormat) const {
			std::stringstream ss;
			int y, m, d; GetDatePart(m_nTicks, y, m, d);
			ss << std::setfill('0') << std::setw(4) << y << "-"
				<< std::setw(2) << m << "-" << std::setw(2) << d << " "
				<< std::setw(2) << GetHour() << ":" << std::setw(2) << GetMinute() << ":" << std::setw(2) << GetSecond();
			return String(ss.str().c_str());
		}

		int DateTime::DaysInMonth(int year, int month) {
			if (month < 1 || month > 12) throw ArgumentOutOfRangeException("Month must be between 1 and 12.");
			const int* days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			return days [month] - days [month - 1];
		}

		bool DateTime::IsLeapYear(int year) {
			if (year < 1 || year > 9999) throw ArgumentOutOfRangeException("Year must be between 1 and 9999.");
			return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
		}

		static std::string ConvertFormat(const String& format) {
			std::string f = format.GetRawString();
			size_t pos = 0;
			while ((pos = f.find("yyyy")) != std::string::npos) f.replace(pos, 4, "%Y");
			while ((pos = f.find("MM")) != std::string::npos) f.replace(pos, 2, "%m");
			while ((pos = f.find("dd")) != std::string::npos) f.replace(pos, 2, "%d");
			while ((pos = f.find("HH")) != std::string::npos) f.replace(pos, 2, "%H");
			while ((pos = f.find("mm")) != std::string::npos) f.replace(pos, 2, "%M");
			while ((pos = f.find("ss")) != std::string::npos) f.replace(pos, 2, "%S");
			return f;
		}

		static bool ParseDateTimeStr(const String& s, const String& format, DateTime& result) {
			std::istringstream ss(s.GetRawString());
			std::tm t = {};
			std::string f = ConvertFormat(format);
			ss >> std::get_time(&t, f.c_str());
			if (ss.fail()) return false;
			try {
				result = DateTime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
				return true;
			}
			catch (...) { return false; }
		}

		bool DateTime::TryParseExact(const String& s, const String& format, DateTime& result) {
			return ParseDateTimeStr(s, format, result);
		}

		bool DateTime::TryParse(const String& s, DateTime& result) {
			return ParseDateTimeStr(s, "yyyy-MM-dd HH:mm:ss", result);
		}

		DateTime DateTime::ParseExact(const String& s, const String& format) {
			DateTime result(1, 1, 1);
			if (!TryParseExact(s, format, result))
				throw FormatException("String was not recognized as a valid DateTime.");
			return result;
		}

		DateTime DateTime::Parse(const String& s) {
			DateTime result(1, 1, 1);
			if (!TryParse(s, result))
				throw FormatException("String was not recognized as a valid DateTime.");
			return result;
		}

	} // namespace System
} // namespace DotNetDupe
