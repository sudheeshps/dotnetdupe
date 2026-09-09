#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {

        /// \class DateTimeOffset
        /// \brief Represents a point in time, typically expressed as a date and time of day, relative to Coordinated Universal Time (UTC).
        ///
        /// \note Conforms to ECMA-335 Partition IV and ISO 8601. Stores an absolute point in time
        ///       as 100-nanosecond ticks since January 1, 0001 00:00:00 UTC.
        class DateTimeOffset : public Object {
        public:
            /// \brief Initializes a new instance of DateTimeOffset to 0 ticks.
            DateTimeOffset() : _ticks(0) {}

            /// \brief Initializes a new instance of DateTimeOffset with the specified number of ticks.
            /// \param ticks A date and time expressed in the number of 100-nanosecond units.
            DateTimeOffset(int64_t ticks) : _ticks(ticks) {}

            /// \brief Gets the number of ticks that represent the date and time of the current DateTimeOffset object in UTC.
            /// \return The number of 100-nanosecond ticks.
            int64_t GetTicks() const { return _ticks; }

            /// \brief Gets a DateTimeOffset object that is set to the current date and time on the current computer, with the offset set to the local time's offset from UTC.
            /// \return A DateTimeOffset object whose date and time is the current local time.
            DOTNETDUPE_API static DateTimeOffset Now();

            /// \brief Gets a DateTimeOffset object whose date and time are set to the current Coordinated Universal Time (UTC).
            /// \return An object whose date and time is the current Coordinated Universal Time (UTC).
            DOTNETDUPE_API static DateTimeOffset UtcNow();

            /// \brief Converts the value of the current DateTimeOffset object to its equivalent string representation.
            /// \return A string representation of the value of the current DateTimeOffset object.
            DOTNETDUPE_API String ToString() const;

            /// \brief Converts the value of the current DateTimeOffset object to its equivalent string representation using the specified format.
            /// \param sFormat A format specification string.
            /// \return A string representation of the current DateTimeOffset object.
            DOTNETDUPE_API String ToString(const String& sFormat) const;

            /// \brief Subtracts a DateTimeOffset from this instance to calculate the elapsed TimeSpan.
            TimeSpan operator-(const DateTimeOffset& other) const {
                return TimeSpan(_ticks - other._ticks);
            }

            /// \brief Indicates whether two DateTimeOffset instances represent the same point in time.
            bool operator==(const DateTimeOffset& other) const { return _ticks == other._ticks; }

            /// \brief Indicates whether two DateTimeOffset instances represent different points in time.
            bool operator!=(const DateTimeOffset& other) const { return _ticks != other._ticks; }

            /// \brief Indicates whether one DateTimeOffset occurs earlier than another.
            bool operator<(const DateTimeOffset& other) const { return _ticks < other._ticks; }

            /// \brief Indicates whether one DateTimeOffset occurs earlier than or at the same time as another.
            bool operator<=(const DateTimeOffset& other) const { return _ticks <= other._ticks; }

            /// \brief Indicates whether one DateTimeOffset occurs later than another.
            bool operator>(const DateTimeOffset& other) const { return _ticks > other._ticks; }

            /// \brief Indicates whether one DateTimeOffset occurs later than or at the same time as another.
            bool operator>=(const DateTimeOffset& other) const { return _ticks >= other._ticks; }

        private:
            int64_t _ticks;
        };
    }
}
