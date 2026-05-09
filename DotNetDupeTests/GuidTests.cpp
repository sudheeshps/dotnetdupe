#include "pch.h"
#include "gtest/gtest.h"
#include "System/Guid.h"

using namespace DotNetDupe::System;

namespace DotNetDupeTests {

    TEST(GuidTest, NewGuid_Should_GenerateUniqueGuids) {
        // Given & When
        Guid g1 = Guid::NewGuid();
        Guid g2 = Guid::NewGuid();

        // Then
        EXPECT_NE(g1, g2);
        EXPECT_NE(g1, Guid::Empty);
    }

    TEST(GuidTest, ToString_Should_ReturnFormattedString) {
        // Given
        Guid g = Guid::NewGuid();

        // When
        String s = g.ToString();

        // Then
        EXPECT_EQ(s.GetLength(), 36);
        EXPECT_TRUE(s.Contains(_T('-')));
    }

    TEST(GuidTest, Constructor_Given_ValidString_When_Called_Then_ParsesCorrectly) {
        // Given
        String guidStr(_T("12345678-1234-1234-1234-1234567890ab"));

        // When
        Guid g(guidStr);

        // Then
        EXPECT_TRUE(g.ToString() == guidStr);
    }

    TEST(GuidTest, Empty_Should_ReturnZeroGuid) {
        // Given
        Guid empty = Guid::Empty;

        // When & Then
        EXPECT_TRUE(empty.ToString() == _T("00000000-0000-0000-0000-000000000000"));
    }
}
