#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"
#include "System/DaylightTime.h"

namespace DotNetDupe {
    namespace System {

        /// \class TimeZone
        /// \brief Represents a time zone and handles time zone conversions.
        ///
        /// Modeled after .NET System.TimeZone. Supports querying standard and daylight names,
        /// daylight changes, UTC offset calculations, and UTC/local time conversions.
        class TimeZone : public Object {
        public:
            /// \brief Gets the daylight saving time zone name.
            /// \return The daylight name string.
            DOTNETDUPE_API virtual String GetDaylightName() const = 0;

            /// \brief Gets the standard time zone name.
            /// \return The standard name string.
            DOTNETDUPE_API virtual String GetStandardName() const = 0;

            /// \brief Gets the time zone of the current computer.
            /// \return Pointer to the local TimeZone singleton instance.
            DOTNETDUPE_API static TimeZone* GetCurrentTimeZone();

            /// \brief Returns the daylight saving time period for a particular year.
            /// \param year The year to evaluate.
            /// \return DaylightTime structure specifying daylight start, end, and delta.
            DOTNETDUPE_API virtual DaylightTime GetDaylightChanges(int year) = 0;

            /// \brief Returns the Coordinated Universal Time (UTC) offset for the specified time.
            /// \param time The point in time to evaluate.
            /// \return A TimeSpan representing the UTC offset.
            DOTNETDUPE_API virtual TimeSpan GetUtcOffset(const DateTimeOffset& time) = 0;

            /// \brief Returns a value indicating whether the specified date and time is within a daylight saving time period.
            /// \param time The date and time to evaluate.
            /// \return True if time falls within daylight saving time; otherwise false.
            DOTNETDUPE_API virtual bool IsDaylightSavingTime(const DateTimeOffset& time);

            /// \brief Returns the local time that corresponds to a specified date and time value.
            /// \param time A DateTimeOffset value.
            /// \return Converted local DateTimeOffset.
            DOTNETDUPE_API virtual DateTimeOffset ToLocalTime(const DateTimeOffset& time);

            /// \brief Returns the Coordinated Universal Time (UTC) that corresponds to a specified time.
            /// \param time A DateTimeOffset value.
            /// \return Converted UTC DateTimeOffset.
            DOTNETDUPE_API virtual DateTimeOffset ToUniversalTime(const DateTimeOffset& time);

            /// \brief Returns a value indicating whether the specified date and time is within the specified daylight saving time period.
            /// \param time A date and time value.
            /// \param daylightTimes A daylight saving time period.
            /// \return True if time is in daylight saving period; otherwise false.
            DOTNETDUPE_API static bool IsDaylightSavingTime(const DateTimeOffset& time, const DaylightTime& daylightTimes);
        };

    }
}
