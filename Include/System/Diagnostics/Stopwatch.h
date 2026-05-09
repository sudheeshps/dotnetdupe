#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/TimeSpan.h"
#include "System/TimeProvider.h"
#include <memory>

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {
            class Stopwatch : public Object {
            public:
                DOTNETDUPE_API Stopwatch();
                DOTNETDUPE_API Stopwatch(const std::shared_ptr<TimeProvider>& timeProvider);

                DOTNETDUPE_API void Start();
                DOTNETDUPE_API void Stop();
                DOTNETDUPE_API void Reset();
                DOTNETDUPE_API void Restart();

                DOTNETDUPE_API bool IsRunning() const { return _isRunning; }
                DOTNETDUPE_API TimeSpan Elapsed() const;
                DOTNETDUPE_API long long ElapsedMilliseconds() const;
                DOTNETDUPE_API long long ElapsedTicks() const;

                DOTNETDUPE_API static Stopwatch StartNew();
                DOTNETDUPE_API static Stopwatch StartNew(const std::shared_ptr<TimeProvider>& timeProvider);
                DOTNETDUPE_API static long long GetTimestamp();
                DOTNETDUPE_API static const long long Frequency;
                DOTNETDUPE_API static const bool IsHighResolution;

            private:
                std::shared_ptr<TimeProvider> _timeProvider;
                long long _elapsedTicks;
                long long _startTimeStamp;
                bool _isRunning;

                long long GetRawElapsedTicks() const;
            };
        }
    }
}
