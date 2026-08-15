#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/TimeSpan.h"
#include "System/TimeProvider.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {
            using TimeProviderPtr = SmartPointer<TimeProvider>;
            class Stopwatch : public Object {
            public:
                DOTNETDUPE_API Stopwatch();
                DOTNETDUPE_API Stopwatch(const TimeProviderPtr& timeProvider);
                DOTNETDUPE_API ~Stopwatch() override;

                DOTNETDUPE_API void Start();
                DOTNETDUPE_API void Stop();
                DOTNETDUPE_API void Reset();
                DOTNETDUPE_API void Restart();

                DOTNETDUPE_API bool IsRunning() const;
                DOTNETDUPE_API TimeSpan Elapsed() const;
                DOTNETDUPE_API long long ElapsedMilliseconds() const;
                DOTNETDUPE_API long long ElapsedTicks() const;

                DOTNETDUPE_API static Stopwatch StartNew();
                DOTNETDUPE_API static Stopwatch StartNew(const TimeProviderPtr& timeProvider);
                DOTNETDUPE_API static long long GetTimestamp();
                DOTNETDUPE_API static const long long Frequency;
                DOTNETDUPE_API static const bool IsHighResolution;

            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                long long GetRawElapsedTicks() const;
            };
        }
    }
}
