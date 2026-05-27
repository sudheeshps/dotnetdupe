#include "pch.h"
#include "gtest/gtest.h"
#include "System/TimeZone.h"
#include "System/DateTimeOffset.h"
#include "System/DaylightTime.h"
#include "System/Char.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace TimeZoneTestCases {
        
        TEST(TimeZoneTest, GetCurrentTimeZone_Should_ReturnNonNull_When_Invoked) {
            // Given: The system is running
            // When: GetCurrentTimeZone is called
            TimeZone* tz = TimeZone::GetCurrentTimeZone();
            // Then: It should not be null
            ASSERT_NE(tz, nullptr);
        }

        TEST(TimeZoneTest, GetStandardName_Should_ReturnNonEmptyString_When_Invoked) {
            // Given: A current time zone
            TimeZone* tz = TimeZone::GetCurrentTimeZone();
            // When: GetStandardName is called
            String name = tz->GetStandardName();
            // Then: It should not be empty
            ASSERT_FALSE(name.IsEmpty());
        }

        TEST(TimeZoneTest, GetUtcOffset_Should_ReturnValidOffset_When_Invoked) {
            // Given: A current time zone and a fixed time
            TimeZone* tz = TimeZone::GetCurrentTimeZone();
            DateTimeOffset now(638200000000000000LL);
            // When: GetUtcOffset is called
            TimeSpan offset = tz->GetUtcOffset(now);
            // Then: It should return a TimeSpan (can be zero, positive, or negative)
            // No specific value to assert without knowing the local machine's TZ, 
            // but we can assert it's within a reasonable range (-14 to +14 hours)
            ASSERT_TRUE(offset.GetTotalHours() >= -14.0 && offset.GetTotalHours() <= 14.0);
        }

        TEST(TimeZoneTest, IsDaylightSavingTime_Should_Work_When_InvokedWithDaylightTime) {
            // Given: A DaylightTime period and a point in time
            DateTimeOffset start(100);
            DateTimeOffset end(200);
            TimeSpan delta = TimeSpan::FromHours(1);
            DaylightTime dt(start, end, delta);
            
            // When: Checking if times are in DST
            // Then:
            ASSERT_TRUE(TimeZone::IsDaylightSavingTime(DateTimeOffset(150), dt));
            ASSERT_FALSE(TimeZone::IsDaylightSavingTime(DateTimeOffset(50), dt));
            ASSERT_FALSE(TimeZone::IsDaylightSavingTime(DateTimeOffset(250), dt));
        }

        TEST(TimeZoneTest, ToLocalTime_Should_AdjustTicksCorrectly_When_Invoked) {
            // Given: A time in UTC
            TimeZone* tz = TimeZone::GetCurrentTimeZone();
            DateTimeOffset utcTime(638200000000000000LL);
            TimeSpan offset = tz->GetUtcOffset(utcTime);
            
            // When: ToLocalTime is called
            DateTimeOffset localTime = tz->ToLocalTime(utcTime);
            
            // Then: The ticks should be adjusted by the offset
            ASSERT_EQ(localTime.GetTicks(), utcTime.GetTicks() + offset.GetTicks());
        }

        TEST(TimeZoneTest, ToUniversalTime_Should_AdjustTicksCorrectly_When_Invoked) {
            // Given: A time in Local
            TimeZone* tz = TimeZone::GetCurrentTimeZone();
            DateTimeOffset localTime(638200000000000000LL);
            TimeSpan offset = tz->GetUtcOffset(localTime);
            
            // When: ToUniversalTime is called
            DateTimeOffset utcTime = tz->ToUniversalTime(localTime);
            
            // Then: The ticks should be adjusted by subtracting the offset
            ASSERT_EQ(utcTime.GetTicks(), localTime.GetTicks() - offset.GetTicks());
        }
    }
}
