#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/TimeSpan.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        class DateTimeOffset : public Object {
        public:
            DateTimeOffset() : _ticks(0) {}
            DateTimeOffset(int64_t ticks) : _ticks(ticks) {}

            int64_t GetTicks() const { return _ticks; }

            TimeSpan operator-(const DateTimeOffset& other) const {
                return TimeSpan(_ticks - other._ticks);
            }

            bool operator==(const DateTimeOffset& other) const { return _ticks == other._ticks; }
            bool operator!=(const DateTimeOffset& other) const { return _ticks != other._ticks; }
            bool operator<(const DateTimeOffset& other) const { return _ticks < other._ticks; }
            bool operator<=(const DateTimeOffset& other) const { return _ticks <= other._ticks; }
            bool operator>(const DateTimeOffset& other) const { return _ticks > other._ticks; }
            bool operator>=(const DateTimeOffset& other) const { return _ticks >= other._ticks; }

        private:
            int64_t _ticks;
        };
    }
}
