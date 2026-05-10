#include "pch.h"
#include "System/TimeZone.h"
#include <windows.h>

namespace DotNetDupe {
    namespace System {
        class CurrentTimeZoneImpl : public TimeZone {
        public:
            String GetDaylightName() const override {
                TIME_ZONE_INFORMATION tzi;
                GetTimeZoneInformation(&tzi);
                return String(tzi.DaylightName);
            }

            String GetStandardName() const override {
                TIME_ZONE_INFORMATION tzi;
                GetTimeZoneInformation(&tzi);
                return String(tzi.StandardName);
            }

            DaylightTime GetDaylightChanges(int year) override {
                TIME_ZONE_INFORMATION tzi;
                GetTimeZoneInformation(&tzi);
                // This is a simplified implementation. Real GetDaylightChanges should calculate the exact dates for the given year.
                // For now, we return a zeroed DaylightTime if we can't easily calculate it.
                return DaylightTime(DateTimeOffset(0), DateTimeOffset(0), TimeSpan(tzi.DaylightBias * -600000000LL));
            }

            TimeSpan GetUtcOffset(const DateTimeOffset& time) override {
                TIME_ZONE_INFORMATION tzi;
                DWORD result = GetTimeZoneInformation(&tzi);
                long totalBias = tzi.Bias;
                if (result == TIME_ZONE_ID_DAYLIGHT) {
                    totalBias += tzi.DaylightBias;
                }
                else if (result == TIME_ZONE_ID_STANDARD) {
                    totalBias += tzi.StandardBias;
                }
                return TimeSpan(totalBias * -600000000LL);
            }
        };

        TimeZone* TimeZone::GetCurrentTimeZone() {
            static CurrentTimeZoneImpl instance;
            return &instance;
        }

        bool TimeZone::IsDaylightSavingTime(const DateTimeOffset& time) {
            TIME_ZONE_INFORMATION tzi;
            DWORD result = GetTimeZoneInformation(&tzi);
            return result == TIME_ZONE_ID_DAYLIGHT;
        }

        DateTimeOffset TimeZone::ToLocalTime(const DateTimeOffset& time) {
            return DateTimeOffset(time.GetTicks() + GetUtcOffset(time).GetTicks());
        }

        DateTimeOffset TimeZone::ToUniversalTime(const DateTimeOffset& time) {
            return DateTimeOffset(time.GetTicks() - GetUtcOffset(time).GetTicks());
        }

        bool TimeZone::IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes) {
            if (daylightTimes.GetDelta().GetTicks() == 0) return false;
            if (daylightTimes.GetStart() == daylightTimes.GetEnd()) return false;
            return time >= daylightTimes.GetStart() && time < daylightTimes.GetEnd();
        }
    }
}
