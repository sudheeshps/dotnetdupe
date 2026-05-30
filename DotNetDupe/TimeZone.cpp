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
#if defined(_WIN32)
                TIME_ZONE_INFORMATION tzi;
                if (GetTimeZoneInformation(&tzi) != TIME_ZONE_ID_INVALID) {
                    return DaylightTime(DateTimeOffset(0), DateTimeOffset(0), TimeSpan(tzi.DaylightBias * -600000000LL));
                }
#endif
                return DaylightTime(DateTimeOffset(0), DateTimeOffset(0), TimeSpan(0));
            }

            TimeSpan GetUtcOffset(const DateTimeOffset& time) override {
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
            static CurrentTimeZoneImpl instance;
            return &instance;
        }

        bool TimeZone::IsDaylightSavingTime(const DateTimeOffset& time) {
#if defined(_WIN32)
            TIME_ZONE_INFORMATION tzi;
            DWORD result = GetTimeZoneInformation(&tzi);
            return result == TIME_ZONE_ID_DAYLIGHT;
#else
            return false;
#endif
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
