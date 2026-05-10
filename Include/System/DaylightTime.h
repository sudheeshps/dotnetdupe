#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/TimeSpan.h"
#include "System/DateTimeOffset.h"

namespace DotNetDupe {
    namespace System {
        class DaylightTime : public Object {
        public:
            DOTNETDUPE_API DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta);

            DOTNETDUPE_API DateTimeOffset GetStart() const;
            DOTNETDUPE_API DateTimeOffset GetEnd() const;
            DOTNETDUPE_API TimeSpan GetDelta() const;

        private:
            DateTimeOffset _start;
            DateTimeOffset _end;
            TimeSpan _delta;
        };
    }
}
