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

            /// \class Stopwatch
            /// \brief Provides a set of methods and properties that you can use to accurately measure elapsed time.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.67 specifications.
            /// Utilizes high-resolution hardware timers (QueryPerformanceCounter on Win32,
            /// CLOCK_MONOTONIC / std::chrono::steady_clock on Linux) to provide sub-microsecond
            /// timing precision without drift.
            class Stopwatch : public Object {
            public:
                /// \brief Initializes a new instance of the Stopwatch class.
                DOTNETDUPE_API Stopwatch();

                /// \brief Initializes a new instance of the Stopwatch class using the specified TimeProvider.
                /// \param timeProvider The TimeProvider to use for timestamp generation.
                DOTNETDUPE_API Stopwatch(const TimeProviderPtr& timeProvider);

                /// \brief Destructor releasing stopwatch resources.
                DOTNETDUPE_API ~Stopwatch() override;

                /// \brief Starts, or resumes, measuring elapsed time for an interval.
                DOTNETDUPE_API void Start();

                /// \brief Stops measuring elapsed time for an interval.
                DOTNETDUPE_API void Stop();

                /// \brief Stops time measurement and resets the elapsed time to zero.
                DOTNETDUPE_API void Reset();

                /// \brief Stops time measurement, resets the elapsed time to zero, and starts measuring elapsed time.
                DOTNETDUPE_API void Restart();

                /// \brief Gets a value indicating whether the Stopwatch timer is running.
                /// \return true if the Stopwatch instance is currently running; otherwise, false.
                DOTNETDUPE_API bool IsRunning() const;

                /// \brief Gets the total elapsed time measured by the current instance.
                /// \return A read-only TimeSpan representing the total elapsed time measured.
                DOTNETDUPE_API TimeSpan Elapsed() const;

                /// \brief Gets the total elapsed time measured by the current instance, in milliseconds.
                /// \return A 64-bit integer representing the total number of milliseconds elapsed.
                DOTNETDUPE_API long long ElapsedMilliseconds() const;

                /// \brief Gets the total elapsed time measured by the current instance, in timer ticks.
                /// \return A 64-bit integer representing the total number of timer ticks elapsed.
                DOTNETDUPE_API long long ElapsedTicks() const;

                /// \brief Initializes a new Stopwatch instance, sets the elapsed time property to zero, and starts measuring elapsed time.
                /// \return A Stopwatch that has just begun measuring elapsed time.
                DOTNETDUPE_API static Stopwatch StartNew();

                /// \brief Initializes a new Stopwatch instance using a TimeProvider, and starts measuring elapsed time.
                /// \param timeProvider The custom time provider.
                /// \return A running Stopwatch instance.
                DOTNETDUPE_API static Stopwatch StartNew(const TimeProviderPtr& timeProvider);

                /// \brief Gets the current number of ticks in the timer mechanism.
                /// \return A 64-bit integer representing the tick counter value of the underlying timer.
                DOTNETDUPE_API static long long GetTimestamp();

                /// \brief Gets the frequency of the timer as the number of ticks per second.
                DOTNETDUPE_API static const long long Frequency;

                /// \brief Indicates whether the timer is based on a high-resolution performance counter.
                DOTNETDUPE_API static const bool IsHighResolution;

            private:
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                long long GetRawElapsedTicks() const;
            };
        }
    }
}
