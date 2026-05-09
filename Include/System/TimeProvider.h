#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"
#include <memory>

namespace DotNetDupe {
    namespace System {
        class TimeProvider : public Object {
        public:
            virtual ~TimeProvider() = default;

            DOTNETDUPE_API virtual DateTimeOffset GetUtcNow() const = 0;
            DOTNETDUPE_API virtual DateTimeOffset GetLocalNow() const = 0;
            DOTNETDUPE_API virtual int64_t GetTimestamp() const = 0;
            DOTNETDUPE_API virtual int64_t GetTimestampFrequency() const = 0;

            DOTNETDUPE_API TimeSpan GetElapsedTime(int64_t startingTimestamp) const;
            DOTNETDUPE_API TimeSpan GetElapsedTime(int64_t startingTimestamp, int64_t endingTimestamp) const;

            DOTNETDUPE_API static std::shared_ptr<TimeProvider> GetSystem();
        };
    }
}
