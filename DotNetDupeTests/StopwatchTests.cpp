#include "pch.h"
#include "gtest/gtest.h"
#include "System/Diagnostics/Stopwatch.h"
#include <thread>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

namespace DotNetDupeTests {

    class MockTimeProviderForStopwatch : public TimeProvider {
    public:
        MockTimeProviderForStopwatch() : _timestamp(0) {}

        DateTimeOffset GetUtcNow() const override { return DateTimeOffset(0); }
        DateTimeOffset GetLocalNow() const override { return DateTimeOffset(0); }
        int64_t GetTimestamp() const override { return _timestamp; }
        int64_t GetTimestampFrequency() const override { return TimeSpan::TicksPerSecond; }

        void AdvanceTimestamp(int64_t ticks) { _timestamp += ticks; }

    private:
        int64_t _timestamp;
    };

    // --- Positive Cases ---

    TEST(StopwatchTest, StartNew_Should_ReturnRunningStopwatch) {
        // Given & When
        Stopwatch sw = Stopwatch::StartNew();

        // Then
        EXPECT_TRUE(sw.IsRunning());
    }

    TEST(StopwatchTest, Elapsed_Should_ReturnCorrectDuration_With_Mock) {
        // Given
        auto mock = SmartPointer<MockTimeProviderForStopwatch>::NewShared();
        Stopwatch sw(mock);

        // When
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond * 2); // 2 seconds
        sw.Stop();

        // Then
        EXPECT_EQ(sw.Elapsed().GetTotalSeconds(), 2.0);
        EXPECT_FALSE(sw.IsRunning());
    }

    TEST(StopwatchTest, Restart_Should_ResetAndStart) {
        // Given
        auto mock = SmartPointer<MockTimeProviderForStopwatch>::NewShared();
        Stopwatch sw(mock);
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);
        sw.Stop();
        EXPECT_EQ(sw.Elapsed().GetTotalSeconds(), 1.0);

        // When
        sw.Restart();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond * 3);

        // Then
        EXPECT_TRUE(sw.IsRunning());
        EXPECT_EQ(sw.Elapsed().GetTotalSeconds(), 3.0);
    }

    TEST(StopwatchTest, Reset_Should_ClearElapsedAndStop) {
        // Given
        auto mock = SmartPointer<MockTimeProviderForStopwatch>::NewShared();
        Stopwatch sw(mock);
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);

        // When
        sw.Reset();

        // Then
        EXPECT_FALSE(sw.IsRunning());
        EXPECT_EQ(sw.Elapsed().GetTicks(), 0);
    }

    // --- Negative / Edge Cases ---

    TEST(StopwatchTest, Stop_When_AlreadyStopped_Should_HaveNoEffect) {
        // Given
        auto mock = SmartPointer<MockTimeProviderForStopwatch>::NewShared();
        Stopwatch sw(mock);
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);
        sw.Stop();
        long long elapsed = sw.ElapsedTicks();

        // When
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);
        sw.Stop();

        // Then
        EXPECT_EQ(sw.ElapsedTicks(), elapsed);
    }

    TEST(StopwatchTest, Start_When_AlreadyRunning_Should_HaveNoEffect) {
        // Given
        auto mock = SmartPointer<MockTimeProviderForStopwatch>::NewShared();
        Stopwatch sw(mock);
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);
        
        // When
        sw.Start();
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond);
        sw.Stop();

        // Then
        EXPECT_EQ(sw.Elapsed().GetTotalSeconds(), 2.0);
    }
}
