#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"
#include "System/DaylightTime.h"

namespace DotNetDupe {
    namespace System {
        class TimeZone : public Object {
        public:
            DOTNETDUPE_API virtual String GetDaylightName() const = 0;
            DOTNETDUPE_API virtual String GetStandardName() const = 0;
            
            DOTNETDUPE_API static TimeZone* GetCurrentTimeZone();

            DOTNETDUPE_API virtual DaylightTime GetDaylightChanges(int year) = 0;
            DOTNETDUPE_API virtual TimeSpan GetUtcOffset(const DateTimeOffset& time) = 0;
            DOTNETDUPE_API virtual bool IsDaylightSavingTime(const DateTimeOffset& time);
            
            DOTNETDUPE_API virtual DateTimeOffset ToLocalTime(const DateTimeOffset& time);
            DOTNETDUPE_API virtual DateTimeOffset ToUniversalTime(const DateTimeOffset& time);

            DOTNETDUPE_API static bool IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes);
        };
    }
}
