#include "pch.h"
#include "gtest/gtest.h"
#include "System/TimeZoneInfo.h"
#include "System/DateTimeOffset.h"
#include <tchar.h>

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace TimeZoneInfoTestCases {

        TEST(TimeZoneInfoTest, Utc_Should_ReturnUtcTimeZone_When_Invoked) {
            // Given: Nothing
            // When: TimeZoneInfo::Utc() is called
            TimeZoneInfo utc = TimeZoneInfo::Utc();
            // Then: Id should be "UTC" and offset should be zero
            ASSERT_EQ(utc.GetId(), String(_T("UTC")));
            ASSERT_EQ(utc.GetBaseUtcOffset().GetTicks(), 0);
        }

        TEST(TimeZoneInfoTest, Local_Should_ReturnLocalTimeZone_When_Invoked) {
            // Given: Nothing
            // When: TimeZoneInfo::Local() is called
            TimeZoneInfo local = TimeZoneInfo::Local();
            // Then: It should have a non-empty Id
            ASSERT_FALSE(local.GetId().IsEmpty());
        }

        TEST(TimeZoneInfoTest, Equals_Should_ReturnTrue_When_SameTimeZonesCompared) {
            // Given: Two UTC time zone objects
            TimeZoneInfo utc1 = TimeZoneInfo::Utc();
            TimeZoneInfo utc2 = TimeZoneInfo::Utc();
            // When: Equals is called
            // Then: It should return true
            ASSERT_TRUE(utc1.Equals(utc2));
        }

        TEST(TimeZoneInfoTest, GetUtcOffset_Should_ReturnZero_When_TimeZoneIsUtc) {
            // Given: UTC time zone
            TimeZoneInfo utc = TimeZoneInfo::Utc();
            DateTimeOffset now(638200000000000000LL);
            // When: GetUtcOffset is called
            TimeSpan offset = utc.GetUtcOffset(now);
            // Then: It should be zero
            ASSERT_EQ(offset.GetTicks(), 0);
        }

        TEST(TimeZoneInfoTest, FindSystemTimeZoneById_Should_ReturnUtc_When_IdIsUtc) {
            // Given: ID "UTC"
            String id(_T("UTC"));
            // When: FindSystemTimeZoneById is called
            TimeZoneInfo tz = TimeZoneInfo::FindSystemTimeZoneById(id);
            // Then: It should be UTC
            ASSERT_EQ(tz.GetId(), _T("UTC"));
        }
        
        TEST(TimeZoneInfoTest, SupportsDaylightSavingTime_Should_ReturnFalse_For_Utc) {
            // Given: UTC time zone
            TimeZoneInfo utc = TimeZoneInfo::Utc();
            // When: SupportsDaylightSavingTime is called
            // Then: It should be false
            ASSERT_FALSE(utc.SupportsDaylightSavingTime());
        }

        TEST(TimeZoneInfoTest, FindSystemTimeZoneById_Should_ReturnUtc_When_InvalidIdSpecified) {
            // Given: An invalid ID
            String invalidId(_T("InvalidID"));
            // When: FindSystemTimeZoneById is called
            TimeZoneInfo tz = TimeZoneInfo::FindSystemTimeZoneById(invalidId);
            // Then: It should return UTC (current fallback)
            ASSERT_EQ(tz.GetId(), _T("UTC"));
        }

        TEST(TimeZoneInfoTest, GetUtcOffset_Should_BeConsistent_For_Utc) {
            // Given: UTC time zone and multiple times
            TimeZoneInfo utc = TimeZoneInfo::Utc();
            DateTimeOffset t1(0);
            DateTimeOffset t2(638200000000000000LL);
            // When: GetUtcOffset is called
            // Then: It should always be zero
            ASSERT_EQ(utc.GetUtcOffset(t1).GetTicks(), 0);
            ASSERT_EQ(utc.GetUtcOffset(t2).GetTicks(), 0);
        }
    }
}
