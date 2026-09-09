#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"
#include <vector>

namespace DotNetDupe {
    namespace System {

        /// \class TimeZoneInfo
        /// \brief Represents any time zone in the world.
        ///
        /// \note Conforms to ECMA-335 Partition IV. Encapsulates UTC offset, display name,
        ///       standard and daylight names, and daylight saving transition rules.
        class TimeZoneInfo : public Object {
        public:
            /// \brief Gets the time zone identifier.
            /// \return The time zone identifier.
            DOTNETDUPE_API String GetId() const;

            /// \brief Gets the general display name that represents the time zone.
            /// \return The time zone's general display name.
            DOTNETDUPE_API String GetDisplayName() const;

            /// \brief Gets the display name for the time zone's standard time.
            /// \return The display name of the time zone's standard time.
            DOTNETDUPE_API String GetStandardName() const;

            /// \brief Gets the display name for the time zone's daylight saving time.
            /// \return The display name for the time zone's daylight saving time.
            DOTNETDUPE_API String GetDaylightName() const;

            /// \brief Gets the time difference between the current time zone's standard time and Coordinated Universal Time (UTC).
            /// \return An object that represents the time difference between the current time zone's standard time and UTC.
            DOTNETDUPE_API TimeSpan GetBaseUtcOffset() const;

            /// \brief Gets a value that indicates whether the time zone has any daylight saving time rules.
            /// \return true if the time zone supports daylight saving time; otherwise, false.
            DOTNETDUPE_API bool SupportsDaylightSavingTime() const;

            /// \brief Gets a TimeZoneInfo object that represents the local time zone.
            /// \return An object that represents the local time zone.
            DOTNETDUPE_API static TimeZoneInfo Local();

            /// \brief Gets a TimeZoneInfo object that represents the Coordinated Universal Time (UTC) zone.
            /// \return An object that represents the UTC zone.
            DOTNETDUPE_API static TimeZoneInfo Utc();

            /// \brief Calculates the offset or difference between the time in this time zone and UTC for a particular point in time.
            /// \param dateTimeOffset The date and time to determine the offset for.
            /// \return An object that represents the time difference between the two time zones.
            DOTNETDUPE_API TimeSpan GetUtcOffset(const DateTimeOffset& dateTimeOffset) const;

            /// \brief Indicates whether a specified date and time falls in the range of daylight saving time for the time zone of the current TimeZoneInfo object.
            /// \param dateTimeOffset A date and time value.
            /// \return true if the dateTimeOffset parameter is a daylight saving time; otherwise, false.
            DOTNETDUPE_API bool IsDaylightSavingTime(const DateTimeOffset& dateTimeOffset) const;

            /// \brief Converts a time to the time in another time zone.
            /// \param dateTimeOffset The date and time to convert.
            /// \param destinationTimeZone The time zone to convert dateTimeOffset to.
            /// \return The date and time in the destination time zone.
            DOTNETDUPE_API static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& destinationTimeZone);

            /// \brief Converts a time to the time in another time zone based on source and destination time zones.
            DOTNETDUPE_API static DateTimeOffset ConvertTime(const DateTimeOffset& dateTimeOffset, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);

            /// \brief Retrieves a TimeZoneInfo object from the registry or system database based on its identifier.
            /// \param id The time zone identifier.
            /// \return An object whose identifier is the value of the id parameter.
            DOTNETDUPE_API static TimeZoneInfo FindSystemTimeZoneById(const String& id);

            /// \brief Determines whether the current TimeZoneInfo object and another TimeZoneInfo object are equal.
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
