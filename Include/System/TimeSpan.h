#pragma once

#include "Common.h"
#include "System/Object.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {

        /// \class TimeSpan
        /// \brief Represents a time interval (duration of time or elapsed time) measured as a positive or negative number of ticks.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.17 (System.TimeSpan).
        ///       Each tick represents a 100-nanosecond interval (0.1 microseconds).
        class TimeSpan : public Object {
        public:
            /// \brief The number of ticks in 1 millisecond.
            static constexpr int64_t TicksPerMillisecond = 10000;

            /// \brief The number of ticks in 1 second.
            static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;

            /// \brief The number of ticks in 1 minute.
            static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;

            /// \brief The number of ticks in 1 hour.
            static constexpr int64_t TicksPerHour = TicksPerMinute * 60;

            /// \brief The number of ticks in 1 day.
            static constexpr int64_t TicksPerDay = TicksPerHour * 24;

            /// \brief Initializes a new instance of TimeSpan to zero duration.
            TimeSpan() : _ticks(0) {}

            /// \brief Initializes a new instance of TimeSpan to the specified number of ticks.
            /// \param ticks A time period expressed in 100-nanosecond units.
            TimeSpan(int64_t ticks) : _ticks(ticks) {}

            /// \brief Gets the number of ticks that represent the value of the current TimeSpan structure.
            /// \return The number of 100-nanosecond ticks.
            int64_t GetTicks() const { return _ticks; }

            /// \brief Gets the value of the current TimeSpan expressed in whole and fractional milliseconds.
            /// \return The total number of milliseconds represented by this instance.
            double GetTotalMilliseconds() const { return (double)_ticks / TicksPerMillisecond; }

            /// \brief Gets the value of the current TimeSpan expressed in whole and fractional seconds.
            /// \return The total number of seconds represented by this instance.
            double GetTotalSeconds() const { return (double)_ticks / TicksPerSecond; }

            /// \brief Gets the value of the current TimeSpan expressed in whole and fractional minutes.
            /// \return The total number of minutes represented by this instance.
            double GetTotalMinutes() const { return (double)_ticks / TicksPerMinute; }

            /// \brief Gets the value of the current TimeSpan expressed in whole and fractional hours.
            /// \return The total number of hours represented by this instance.
            double GetTotalHours() const { return (double)_ticks / TicksPerHour; }

            /// \brief Gets the value of the current TimeSpan expressed in whole and fractional days.
            /// \return The total number of days represented by this instance.
            double GetTotalDays() const { return (double)_ticks / TicksPerDay; }

            /// \brief Returns a TimeSpan that represents a specified number of ticks.
            /// \param ticks A number of 100-nanosecond ticks.
            /// \return An object that represents ticks.
            static TimeSpan FromTicks(int64_t ticks) { return TimeSpan(ticks); }

            /// \brief Returns a TimeSpan that represents a specified number of milliseconds.
            /// \param value A number of milliseconds, accurate to the nearest millisecond.
            /// \return An object that represents value.
            static TimeSpan FromMilliseconds(double value) { return TimeSpan((int64_t)(value * TicksPerMillisecond)); }

            /// \brief Returns a TimeSpan that represents a specified number of seconds.
            /// \param value A number of seconds, accurate to the nearest millisecond.
            /// \return An object that represents value.
            static TimeSpan FromSeconds(double value) { return TimeSpan((int64_t)(value * TicksPerSecond)); }

            /// \brief Returns a TimeSpan that represents a specified number of minutes.
            /// \param value A number of minutes, accurate to the nearest millisecond.
            /// \return An object that represents value.
            static TimeSpan FromMinutes(double value) { return TimeSpan((int64_t)(value * TicksPerMinute)); }

            /// \brief Returns a TimeSpan that represents a specified number of hours.
            /// \param value A number of hours, accurate to the nearest millisecond.
            /// \return An object that represents value.
            static TimeSpan FromHours(double value) { return TimeSpan((int64_t)(value * TicksPerHour)); }

            /// \brief Returns a TimeSpan that represents a specified number of days.
            /// \param value A number of days, accurate to the nearest millisecond.
            /// \return An object that represents value.
            static TimeSpan FromDays(double value) { return TimeSpan((int64_t)(value * TicksPerDay)); }

            /// \brief Indicates whether two TimeSpan instances are equal.
            bool operator==(const TimeSpan& other) const { return _ticks == other._ticks; }

            /// \brief Indicates whether two TimeSpan instances are not equal.
            bool operator!=(const TimeSpan& other) const { return _ticks != other._ticks; }

            /// \brief Indicates whether a specified TimeSpan is less than another specified TimeSpan.
            bool operator<(const TimeSpan& other) const { return _ticks < other._ticks; }

            /// \brief Indicates whether a specified TimeSpan is less than or equal to another specified TimeSpan.
            bool operator<=(const TimeSpan& other) const { return _ticks <= other._ticks; }

            /// \brief Indicates whether a specified TimeSpan is greater than another specified TimeSpan.
            bool operator>(const TimeSpan& other) const { return _ticks > other._ticks; }

            /// \brief Indicates whether a specified TimeSpan is greater than or equal to another specified TimeSpan.
            bool operator>=(const TimeSpan& other) const { return _ticks >= other._ticks; }

            /// \brief Adds a specified TimeSpan to this instance.
            TimeSpan operator+(const TimeSpan& other) const { return TimeSpan(_ticks + other._ticks); }

            /// \brief Subtracts a specified TimeSpan from this instance.
            TimeSpan operator-(const TimeSpan& other) const { return TimeSpan(_ticks - other._ticks); }

        private:
            int64_t _ticks;
        };
    }
}
