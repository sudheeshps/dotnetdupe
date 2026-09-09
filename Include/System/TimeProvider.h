/// \file TimeProvider.h
/// \brief Provides an abstraction for time, timestamps, and elapsed time calculation.
///
/// Standard Citation: .NET 8 System.TimeProvider.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {

        class TimeProvider;

        /// \typedef TimeProviderPtr
        /// \brief Alias for a smart pointer to a TimeProvider.
        using TimeProviderPtr = SmartPointer<TimeProvider>;

        /// \class TimeProvider
        /// \brief An abstraction that provides time-related functions to enable testability of time-dependent code.
        ///
        /// Modeled after .NET 8 System.TimeProvider. Provides mockable access to high-resolution timestamps,
        /// UTC now, local now, and elapsed duration calculations.
        ///
        /// \note Thread Safety: TimeProvider implementations are thread-safe.
        class TimeProvider : public Object {
        public:
            /// \brief Virtual destructor for polymorphic cleanup.
            virtual ~TimeProvider() = default;

            /// \brief Gets a DateTimeOffset value that is set to the current Coordinated Universal Time (UTC) date and time.
            /// \return Current UTC DateTimeOffset.
            DOTNETDUPE_API virtual DateTimeOffset GetUtcNow() const = 0;

            /// \brief Gets a DateTimeOffset value that is set to the current date and time in the local time zone.
            /// \return Current local DateTimeOffset.
            DOTNETDUPE_API virtual DateTimeOffset GetLocalNow() const = 0;

            /// \brief Gets the current high-frequency counter value in ticks.
            /// \return An integer representing high-frequency timer ticks.
            DOTNETDUPE_API virtual int64_t GetTimestamp() const = 0;

            /// \brief Gets the frequency of the high-frequency counter in ticks per second.
            /// \return Frequency in Hertz.
            DOTNETDUPE_API virtual int64_t GetTimestampFrequency() const = 0;

            /// \brief Gets the elapsed time since the startingTimestamp was recorded.
            /// \param startingTimestamp The timestamp obtained from a previous call to GetTimestamp.
            /// \return A TimeSpan representing elapsed duration.
            DOTNETDUPE_API TimeSpan GetElapsedTime(int64_t startingTimestamp) const;

            /// \brief Gets the elapsed time between two timestamp values.
            /// \param startingTimestamp The initial timestamp value.
            /// \param endingTimestamp The final timestamp value.
            /// \return A TimeSpan representing the duration between the timestamps.
            DOTNETDUPE_API TimeSpan GetElapsedTime(int64_t startingTimestamp, int64_t endingTimestamp) const;

            /// \brief Gets a TimeProvider that provides the system's current time and high-resolution timer.
            /// \return The default system TimeProvider singleton.
            DOTNETDUPE_API static TimeProviderPtr GetSystem();
        };

    }
}
