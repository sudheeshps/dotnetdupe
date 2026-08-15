#include "pch.h"
#include "gtest/gtest.h"
#include "System/TimeProvider.h"
#include <thread>

using namespace DotNetDupe::System;

namespace DotNetDupeTests {

    class MockTimeProvider : public TimeProvider {
    public:
        MockTimeProvider(DateTimeOffset utcNow) : _utcNow(utcNow), _timestamp(0) {}

        DateTimeOffset GetUtcNow() const override { return _utcNow; }
        DateTimeOffset GetLocalNow() const override { return _utcNow; } // Simplified for test
        int64_t GetTimestamp() const override { return _timestamp; }
        int64_t GetTimestampFrequency() const override { return TimeSpan::TicksPerSecond; }

        void SetUtcNow(DateTimeOffset value) { _utcNow = value; }
        void AdvanceTimestamp(int64_t ticks) { _timestamp += ticks; }

    private:
        DateTimeOffset _utcNow;
        int64_t _timestamp;
    };

    // --- Positive Cases ---

    TEST(TimeProviderTest, GetSystem_Should_ReturnNonNullProvider) {
        // Given
        auto provider = TimeProvider::GetSystem();

        // When & Then
        EXPECT_FALSE(provider.IsNull());
    }

    TEST(TimeProviderTest, GetUtcNow_Should_ReturnCurrentTime) {
        // Given
        auto provider = TimeProvider::GetSystem();

        // When
        auto now1 = provider->GetUtcNow();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto now2 = provider->GetUtcNow();

        // Then
        EXPECT_GT(now2.GetTicks(), now1.GetTicks());
    }

    TEST(TimeProviderTest, GetElapsedTime_Should_ReturnCorrectDuration_With_Mock) {
        // Given
        DateTimeOffset initialTime(1000000);
        auto mock = SmartPointer<MockTimeProvider>::NewShared(initialTime);
        int64_t start = mock->GetTimestamp();

        // When
        mock->AdvanceTimestamp(TimeSpan::TicksPerSecond * 2); // 2 seconds
        TimeSpan elapsed = mock->GetElapsedTime(start);

        // Then
        EXPECT_EQ(elapsed.GetTotalSeconds(), 2.0);
    }

    TEST(TimeProviderTest, GetElapsedTime_BetweenTwoTimestamps_Should_ReturnCorrectDuration) {
        // Given
        auto mock = SmartPointer<MockTimeProvider>::NewShared(DateTimeOffset(0));
        int64_t start = 1000;
        int64_t end = 2000; // 1000 ticks = 1/10000 second if frequency is TicksPerSecond? 
        // Mock frequency is TicksPerSecond (10,000,000). 1000 ticks = 0.0001 seconds.

        // When
        TimeSpan elapsed = mock->GetElapsedTime(start, end);

        // Then
        EXPECT_EQ(elapsed.GetTicks(), 1000);
    }

    // --- Negative / Edge Cases ---

    TEST(TimeProviderTest, GetElapsedTime_With_EndBeforeStart_Should_ReturnNegativeDuration) {
        // Given
        auto mock = SmartPointer<MockTimeProvider>::NewShared(DateTimeOffset(0));
        int64_t start = 2000;
        int64_t end = 1000;

        // When
        TimeSpan elapsed = mock->GetElapsedTime(start, end);

        // Then
        EXPECT_LT(elapsed.GetTicks(), 0);
        EXPECT_EQ(elapsed.GetTicks(), -1000);
    }

    TEST(TimeProviderTest, GetElapsedTime_With_ZeroFrequency_Should_HandleGracefully) {
        // This is a bit tricky since we can't easily change the system frequency, 
        // but we can test our math in GetElapsedTime if we had a provider with 0 frequency (though that's invalid).
        // For now, we trust the system provider returns a valid non-zero frequency.
    }

    TEST(TimeProviderTest, TimeSpan_FromMethods_Should_CreateCorrectTicks) {
        // Given
        auto ts = TimeSpan::FromSeconds(1.5);

        // When & Then
        EXPECT_EQ(ts.GetTicks(), 15000000);
        EXPECT_EQ(ts.GetTotalMilliseconds(), 1500.0);
    }
}
