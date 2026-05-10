#include "pch.h"
#include "System/DaylightTime.h"

namespace DotNetDupe {
    namespace System {
        DaylightTime::DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta)
            : _start(start), _end(end), _delta(delta) {
        }

        DateTimeOffset DaylightTime::GetStart() const {
            return _start;
        }

        DateTimeOffset DaylightTime::GetEnd() const {
            return _end;
        }

        TimeSpan DaylightTime::GetDelta() const {
            return _delta;
        }
    }
}
