#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"

namespace DotNetDupe {
    namespace System {

        /// \class DaylightTime
        /// \brief Defines the period of daylight saving time for a region.
        ///
        /// Modeled after .NET System.Globalization.DaylightTime. Encapsulates start, end,
        /// and delta duration of daylight saving adjustments.
        class DaylightTime : public Object {
        public:
            /// \brief Initializes a new instance of the DaylightTime class with specified start, end, and time delta.
            /// \param start The date and time when the daylight saving period begins.
            /// \param end The date and time when the daylight saving period ends.
            /// \param delta The difference between standard time and daylight saving time.
            DOTNETDUPE_API DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta);

            /// \brief Gets the date and time when the daylight saving period begins.
            /// \return The starting DateTimeOffset.
            DOTNETDUPE_API DateTimeOffset GetStart() const;

            /// \brief Gets the date and time when the daylight saving period ends.
            /// \return The ending DateTimeOffset.
            DOTNETDUPE_API DateTimeOffset GetEnd() const;

            /// \brief Gets the time interval that represents the difference between standard time and daylight saving time.
            /// \return The adjustment TimeSpan.
            DOTNETDUPE_API TimeSpan GetDelta() const;

        private:
            DateTimeOffset _start;
            DateTimeOffset _end;
            TimeSpan _delta;
        };

    }
}
