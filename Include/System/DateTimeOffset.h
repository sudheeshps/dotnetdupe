#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        class DateTimeOffset : public Object {
        public:
            DateTimeOffset() : _ticks(0) {}
            DateTimeOffset(int64_t ticks) : _ticks(ticks) {}

            int64_t GetTicks() const { return _ticks; }

            DOTNETDUPE_API static DateTimeOffset Now();
            DOTNETDUPE_API static DateTimeOffset UtcNow();

            DOTNETDUPE_API String ToString() const;
            DOTNETDUPE_API String ToString(const String& sFormat) const;

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
