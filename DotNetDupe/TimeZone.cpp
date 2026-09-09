#include "pch.h"
#include "System/TimeZone.h"

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#endif

namespace DotNetDupe {
    namespace System {
        class CurrentTimeZoneImpl : public TimeZone {
        public:
            String GetDaylightName() const override {
                /// Step: Retrieve daylight saving zone name from platform API.
#if defined(_WIN32)
                TIME_ZONE_INFORMATION tzi;
                if (GetTimeZoneInformation(&tzi) != TIME_ZONE_ID_INVALID) {
                    return String(WCharToUtf8(tzi.DaylightName).c_str());
                }
                return String("Daylight");
#else
                return String("UTC");
#endif
            }

            String GetStandardName() const override {
                /// Step: Retrieve standard zone name from platform API.
#if defined(_WIN32)
                TIME_ZONE_INFORMATION tzi;
                if (GetTimeZoneInformation(&tzi) != TIME_ZONE_ID_INVALID) {
                    return String(WCharToUtf8(tzi.StandardName).c_str());
                }
                return String("Standard");
#else
                return String("UTC");
#endif
            }

            DaylightTime GetDaylightChanges(int year) override {
                /// Step: Retrieve daylight changes for the current year.
#if defined(_WIN32)
                TIME_ZONE_INFORMATION tzi;
                if (GetTimeZoneInformation(&tzi) != TIME_ZONE_ID_INVALID) {
                    return DaylightTime(DateTimeOffset(0), DateTimeOffset(0), TimeSpan(tzi.DaylightBias * -600000000LL));
                }
#endif
                return DaylightTime(DateTimeOffset(0), DateTimeOffset(0), TimeSpan(0));
            }

            TimeSpan GetUtcOffset(const DateTimeOffset& time) override {
                /// Step: Compute effective UTC offset including bias and daylight adjustment.
#if defined(_WIN32)
                TIME_ZONE_INFORMATION tzi;
                DWORD result = GetTimeZoneInformation(&tzi);
                if (result != TIME_ZONE_ID_INVALID) {
                    long totalBias = tzi.Bias;
                    if (result == TIME_ZONE_ID_DAYLIGHT) {
                        totalBias += tzi.DaylightBias;
                    }
                    else if (result == TIME_ZONE_ID_STANDARD) {
                        totalBias += tzi.StandardBias;
                    }
                    return TimeSpan(totalBias * -600000000LL);
                }
#endif
                return TimeSpan(0);
            }
        };

        TimeZone* TimeZone::GetCurrentTimeZone() {
            /// Retrieve singleton current time zone instance.
            static CurrentTimeZoneImpl instance;
            return &instance;
        }

        bool TimeZone::IsDaylightSavingTime(const DateTimeOffset& time) {
            /// Step: Check if current time falls within active daylight saving period.
#if defined(_WIN32)
            TIME_ZONE_INFORMATION tzi;
            DWORD result = GetTimeZoneInformation(&tzi);
            return result == TIME_ZONE_ID_DAYLIGHT;
#else
            return false;
#endif
        }

        DateTimeOffset TimeZone::ToLocalTime(const DateTimeOffset& time) {
            /// Step: Convert UTC time to local time by adding current UTC offset.
            return DateTimeOffset(time.GetTicks() + GetUtcOffset(time).GetTicks());
        }

        DateTimeOffset TimeZone::ToUniversalTime(const DateTimeOffset& time) {
            /// Step: Convert local time to universal time by subtracting current UTC offset.
            return DateTimeOffset(time.GetTicks() - GetUtcOffset(time).GetTicks());
        }

        bool TimeZone::IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes) {
            /// Guard: If delta is zero or start equals end, daylight saving is not active.
            if (daylightTimes.GetDelta().GetTicks() == 0) return false;
            if (daylightTimes.GetStart() == daylightTimes.GetEnd()) return false;
            /// Check if time falls between daylight transition start and end bounds.
            return time >= daylightTimes.GetStart() && time < daylightTimes.GetEnd();
        }
    }
}
