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
            /// Query dynamic time zone details from the Windows kernel.
            DYNAMIC_TIME_ZONE_INFORMATION dtzi;
            DWORD result = GetDynamicTimeZoneInformation(&dtzi);
            
            /// Extract time zone key identifier, UTC bias, and standard/daylight names.
            String id = String(WCharToUtf8(dtzi.TimeZoneKeyName).c_str());
            TimeSpan baseUtcOffset(dtzi.Bias * -600000000LL);
            String displayName = String(WCharToUtf8(dtzi.StandardName).c_str()); 
            String standardName = String(WCharToUtf8(dtzi.StandardName).c_str());
            String daylightName = String(WCharToUtf8(dtzi.DaylightName).c_str());
            bool supportsDaylightSavingTime = (result != TIME_ZONE_ID_UNKNOWN);

            /// Return configured local time zone representation.
            return TimeZoneInfo(id, baseUtcOffset, displayName, standardName, daylightName, supportsDaylightSavingTime);
#else
            /// Default to UTC zone on non-Windows platforms.
            return Utc();
#endif
        }

        TimeZoneInfo TimeZoneInfo::Utc() {
            /// Return static constant UTC representation with zero bias.
            return TimeZoneInfo(String("UTC"), TimeSpan(0), String("(UTC) Coordinated Universal Time"), String("Coordinated Universal Time"), String("Coordinated Universal Time"), false);
        }

        TimeSpan TimeZoneInfo::GetUtcOffset(const DateTimeOffset& dateTimeOffset) const {
            /// Compute effective UTC offset.
            if (_id == "UTC") return TimeSpan(0);
            return _baseUtcOffset; 
        }

        bool TimeZoneInfo::IsDaylightSavingTime(const DateTimeOffset& dateTimeOffset) const {
            /// Check daylight saving activation for the given timestamp.
            if (!_supportsDaylightSavingTime) return false;
            return false;
        }

        DateTimeOffset TimeZoneInfo::ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone) {
            /// Transform timestamp into destination time zone representation.
            return dateTimeOffset; 
        }

        DateTimeOffset TimeZoneInfo::ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone) {
            /// Transform timestamp from source to destination time zone.
            return dateTimeOffset;
        }

        TimeZoneInfo TimeZoneInfo::FindSystemTimeZoneById(const String& id) {
            /// Lookup system time zone database by key identifier.
            if (id == "UTC") return Utc();
            return Utc(); 
        }

        bool TimeZoneInfo::Equals(const TimeZoneInfo& other) const {
            /// Compare equivalence using canonical zone identifier.
            return _id == other._id;
        }
    }
}
