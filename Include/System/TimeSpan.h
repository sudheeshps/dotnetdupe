#pragma once

#include "Common.h"
#include "System/Object.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        class TimeSpan : public Object {
        public:
            static constexpr int64_t TicksPerMillisecond = 10000;
            static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
            static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
            static constexpr int64_t TicksPerHour = TicksPerMinute * 60;
            static constexpr int64_t TicksPerDay = TicksPerHour * 24;

            TimeSpan() : _ticks(0) {}
            TimeSpan(int64_t ticks) : _ticks(ticks) {}

            int64_t GetTicks() const { return _ticks; }

            double GetTotalMilliseconds() const { return (double)_ticks / TicksPerMillisecond; }
            double GetTotalSeconds() const { return (double)_ticks / TicksPerSecond; }
            double GetTotalMinutes() const { return (double)_ticks / TicksPerMinute; }
            double GetTotalHours() const { return (double)_ticks / TicksPerHour; }
            double GetTotalDays() const { return (double)_ticks / TicksPerDay; }

            static TimeSpan FromTicks(int64_t ticks) { return TimeSpan(ticks); }
            static TimeSpan FromMilliseconds(double value) { return TimeSpan((int64_t)(value * TicksPerMillisecond)); }
            static TimeSpan FromSeconds(double value) { return TimeSpan((int64_t)(value * TicksPerSecond)); }
            static TimeSpan FromMinutes(double value) { return TimeSpan((int64_t)(value * TicksPerMinute)); }
            static TimeSpan FromHours(double value) { return TimeSpan((int64_t)(value * TicksPerHour)); }
            static TimeSpan FromDays(double value) { return TimeSpan((int64_t)(value * TicksPerDay)); }

            bool operator==(const TimeSpan& other) const { return _ticks == other._ticks; }
            bool operator!=(const TimeSpan& other) const { return _ticks != other._ticks; }
            bool operator<(const TimeSpan& other) const { return _ticks < other._ticks; }
            bool operator<=(const TimeSpan& other) const { return _ticks <= other._ticks; }
            bool operator>(const TimeSpan& other) const { return _ticks > other._ticks; }
            bool operator>=(const TimeSpan& other) const { return _ticks >= other._ticks; }

            TimeSpan operator+(const TimeSpan& other) const { return TimeSpan(_ticks + other._ticks); }
            TimeSpan operator-(const TimeSpan& other) const { return TimeSpan(_ticks - other._ticks); }

        private:
            int64_t _ticks;
        };
    }
}
