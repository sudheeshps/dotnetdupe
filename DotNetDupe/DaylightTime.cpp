#include "pch.h"
#include "System/DaylightTime.h"

namespace DotNetDupe {
    namespace System {
        DaylightTime::DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta)
            : _start(start), _end(end), _delta(delta) {
            /// Initialize DaylightTime instance with transition start, end, and offset delta.
        }

        DateTimeOffset DaylightTime::GetStart() const {
            /// Return starting DateTimeOffset of daylight saving transition.
            return _start;
        }

        DateTimeOffset DaylightTime::GetEnd() const {
            /// Return ending DateTimeOffset of daylight saving transition.
            return _end;
        }

        TimeSpan DaylightTime::GetDelta() const {
            /// Return time offset delta applied during daylight saving.
            return _delta;
        }
    }
}

