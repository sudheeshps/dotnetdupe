#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        class TimeZoneInfo : public Object {
        public:
            DOTNETDUPE_API String GetId() const;
            DOTNETDUPE_API String GetDisplayName() const;
            DOTNETDUPE_API String GetStandardName() const;
            DOTNETDUPE_API String GetDaylightName() const;
            DOTNETDUPE_API TimeSpan GetBaseUtcOffset() const;
            DOTNETDUPE_API bool SupportsDaylightSavingTime() const;

            DOTNETDUPE_API static TimeZoneInfo Local();
            DOTNETDUPE_API static TimeZoneInfo Utc();

            DOTNETDUPE_API TimeSpan GetUtcOffset(const DateTimeOffset& dateTimeOffset) const;
            DOTNETDUPE_API bool IsDaylightSavingTime(const DateTimeOffset& dateTimeOffset) const;

            DOTNETDUPE_API static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone);
            DOTNETDUPE_API static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);

            DOTNETDUPE_API static TimeZoneInfo FindSystemTimeZoneById(const String& id);
            
            DOTNETDUPE_API bool Equals(const TimeZoneInfo& other) const;

        private:
            TimeZoneInfo(String id, TimeSpan baseUtcOffset, String displayName, String standardName, String daylightName, bool supportsDaylightSavingTime);
            
            String _id;
            TimeSpan _baseUtcOffset;
            String _displayName;
            String _standardName;
            String _daylightName;
            bool _supportsDaylightSavingTime;
        };
    }
}
