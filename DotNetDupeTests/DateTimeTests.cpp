#include "pch.h"
#include "gtest/gtest.h"
#include "System/DateTime.h"
#include "System/ArgumentOutOfRangeException.h"

using namespace DotNetDupe::System;

class DateTimeTests : public ::testing::Test {};

TEST_F(DateTimeTests, Constructor_GivenValidDate_ReturnsDateTime) {
    DateTime dt(2023, 10, 27);
    EXPECT_EQ(2023, dt.GetYear());
    EXPECT_EQ(10, dt.GetMonth());
    EXPECT_EQ(27, dt.GetDay());
    EXPECT_EQ(0, dt.GetHour());
    EXPECT_EQ(0, dt.GetMinute());
    EXPECT_EQ(0, dt.GetSecond());
}

TEST_F(DateTimeTests, Constructor_GivenValidTime_ReturnsDateTime) {
    DateTime dt(2023, 10, 27, 14, 30, 45);
    EXPECT_EQ(2023, dt.GetYear());
    EXPECT_EQ(10, dt.GetMonth());
    EXPECT_EQ(27, dt.GetDay());
    EXPECT_EQ(14, dt.GetHour());
    EXPECT_EQ(30, dt.GetMinute());
    EXPECT_EQ(45, dt.GetSecond());
}

TEST_F(DateTimeTests, IsLeapYear_GivenLeapYear_ReturnsTrue) {
    EXPECT_TRUE(DateTime::IsLeapYear(2024));
    EXPECT_TRUE(DateTime::IsLeapYear(2000));
    EXPECT_FALSE(DateTime::IsLeapYear(1900));
    EXPECT_FALSE(DateTime::IsLeapYear(2023));
}

TEST_F(DateTimeTests, AddDays_GivenPositiveValue_AddsDays) {
    DateTime dt(2023, 1, 1);
    DateTime dt2 = dt.AddDays(31);
    EXPECT_EQ(2023, dt2.GetYear());
    EXPECT_EQ(2, dt2.GetMonth());
    EXPECT_EQ(1, dt2.GetDay());
}

TEST_F(DateTimeTests, AddMonths_GivenPositiveValue_AddsMonths) {
    DateTime dt(2023, 1, 31);
    DateTime dt2 = dt.AddMonths(1); // Should clamp to Feb 28
    EXPECT_EQ(2023, dt2.GetYear());
    EXPECT_EQ(2, dt2.GetMonth());
    EXPECT_EQ(28, dt2.GetDay());
}

TEST_F(DateTimeTests, ToString_ReturnsFormattedString) {
    DateTime dt(2023, 10, 27, 14, 30, 45);
    String s = dt.ToString();
    EXPECT_STREQ("2023-10-27 14:30:45", s.GetRawString());
}

TEST_F(DateTimeTests, UtcNow_ReturnsValidTime) {
    DateTime dt = DateTime::UtcNow();
    EXPECT_GT(dt.GetYear(), 2020);
    EXPECT_EQ(DateTimeKind::Utc, dt.GetKind());
}

TEST_F(DateTimeTests, InvalidDate_ThrowsException) {
    EXPECT_THROW(DateTime(2023, 2, 29), ArgumentOutOfRangeException);
}

TEST_F(DateTimeTests, Parse_GivenValidString_ReturnsDateTime) {
    DateTime dt = DateTime::Parse("2023-10-27 14:30:45");
    EXPECT_EQ(2023, dt.GetYear());
    EXPECT_EQ(10, dt.GetMonth());
    EXPECT_EQ(27, dt.GetDay());
    EXPECT_EQ(14, dt.GetHour());
    EXPECT_EQ(30, dt.GetMinute());
    EXPECT_EQ(45, dt.GetSecond());
}

TEST_F(DateTimeTests, ParseExact_GivenValidString_ReturnsDateTime) {
    DateTime dt = DateTime::ParseExact("10-27-2023 14:30:45", "MM-dd-yyyy HH:mm:ss");
    EXPECT_EQ(2023, dt.GetYear());
    EXPECT_EQ(10, dt.GetMonth());
    EXPECT_EQ(27, dt.GetDay());
    EXPECT_EQ(14, dt.GetHour());
    EXPECT_EQ(30, dt.GetMinute());
    EXPECT_EQ(45, dt.GetSecond());
}

TEST_F(DateTimeTests, Parse_GivenInvalidString_ThrowsFormatException) {
    EXPECT_THROW(DateTime::Parse("invalid-date"), FormatException);
}

TEST_F(DateTimeTests, TryParse_GivenValidString_ReturnsTrue) {
    DateTime dt(1, 1, 1);
    bool success = DateTime::TryParse("2023-10-27 14:30:45", dt);
    EXPECT_TRUE(success);
    EXPECT_EQ(2023, dt.GetYear());
    EXPECT_EQ(10, dt.GetMonth());
}

TEST_F(DateTimeTests, TryParse_GivenInvalidString_ReturnsFalse) {
    DateTime dt(1, 1, 1);
    bool success = DateTime::TryParse("invalid-date", dt);
    EXPECT_FALSE(success);
}
