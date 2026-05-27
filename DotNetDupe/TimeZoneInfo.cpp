#include "pch.h"
#include "System/TimeZoneInfo.h"
#include "System/Char.h"

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#endif

namespace DotNetDupe {
    namespace System {
        TimeZoneInfo::TimeZoneInfo(String id, TimeSpan baseUtcOffset, String displayName, String standardName, String daylightName, bool supportsDaylightSavingTime)
            : _id(id), _baseUtcOffset(baseUtcOffset), _displayName(displayName), _standardName(standardName), _daylightName(daylightName), _supportsDaylightSavingTime(supportsDaylightSavingTime) {
        }

        String TimeZoneInfo::GetId() const { return _id; }
        String TimeZoneInfo::GetDisplayName() const { return _displayName; }
        String TimeZoneInfo::GetStandardName() const { return _standardName; }
        String TimeZoneInfo::GetDaylightName() const { return _daylightName; }
        TimeSpan TimeZoneInfo::GetBaseUtcOffset() const { return _baseUtcOffset; }
        bool TimeZoneInfo::SupportsDaylightSavingTime() const { return _supportsDaylightSavingTime; }

        TimeZoneInfo TimeZoneInfo::Local() {
#if defined(_WIN32)
            DYNAMIC_TIME_ZONE_INFORMATION dtzi;
            DWORD result = GetDynamicTimeZoneInformation(&dtzi);
            
            String id = String(WCharToUtf8(dtzi.TimeZoneKeyName).c_str());
            TimeSpan baseUtcOffset(dtzi.Bias * -600000000LL);
            String displayName = String(WCharToUtf8(dtzi.StandardName).c_str()); 
            String standardName = String(WCharToUtf8(dtzi.StandardName).c_str());
            String daylightName = String(WCharToUtf8(dtzi.DaylightName).c_str());
            bool supportsDaylightSavingTime = (result != TIME_ZONE_ID_UNKNOWN);

            return TimeZoneInfo(id, baseUtcOffset, displayName, standardName, daylightName, supportsDaylightSavingTime);
#else
            return Utc(); // Placeholder for non-Windows
#endif
        }

        TimeZoneInfo TimeZoneInfo::Utc() {
            return TimeZoneInfo(String("UTC"), TimeSpan(0), String("(UTC) Coordinated Universal Time"), String("Coordinated Universal Time"), String("Coordinated Universal Time"), false);
        }

        TimeSpan TimeZoneInfo::GetUtcOffset(const DateTimeOffset& dateTimeOffset) const {
            if (_id == "UTC") return TimeSpan(0);
            return _baseUtcOffset; 
        }

        bool TimeZoneInfo::IsDaylightSavingTime(const DateTimeOffset& dateTimeOffset) const {
            if (!_supportsDaylightSavingTime) return false;
            return false;
        }

        DateTimeOffset TimeZoneInfo::ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone) {
            return dateTimeOffset; 
        }

        DateTimeOffset TimeZoneInfo::ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone) {
            return dateTimeOffset;
        }

        TimeZoneInfo TimeZoneInfo::FindSystemTimeZoneById(const String& id) {
            if (id == "UTC") return Utc();
            return Utc(); 
        }

        bool TimeZoneInfo::Equals(const TimeZoneInfo& other) const {
            return _id == other._id;
        }
    }
}
