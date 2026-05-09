#include "pch.h"
#include "gtest/gtest.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    // --- String Enhancement Tests ---

    TEST(StringEnhancementTest, ToLower_Should_ConvertToUpperToLower) {
        // Given
        String s(_T("HELLO World"));

        // When
        String lower = s.ToLower();

        // Then
        EXPECT_TRUE(lower == _T("hello world"));
    }

    TEST(StringEnhancementTest, ToUpper_Should_ConvertToLowerToUpper) {
        // Given
        String s(_T("hello World"));

        // When
        String upper = s.ToUpper();

        // Then
        EXPECT_TRUE(upper == _T("HELLO WORLD"));
    }

    TEST(StringEnhancementTest, Trim_Should_RemoveSurroundingWhitespace) {
        // Given
        String s(_T("  \t Hello \r\n  "));

        // When
        String trimmed = s.Trim();

        // Then
        EXPECT_TRUE(trimmed == _T("Hello"));
    }

    TEST(StringEnhancementTest, Split_With_TrimEntries_Should_TrimEachEntry) {
        // Given
        String s(_T("  Apple ,  Banana , Cherry  "));
        String sep [] = { _T(",") };

        // When
        auto parts = s.Split(sep, 1, StringSplitOptions::TrimEntries);

        // Then
        ASSERT_EQ(parts.size(), 3);
        EXPECT_TRUE(parts [0] == _T("Apple"));
        EXPECT_TRUE(parts [1] == _T("Banana"));
        EXPECT_TRUE(parts [2] == _T("Cherry"));
    }

    // --- List Enhancement Tests ---

    TEST(ListEnhancementTest, Exists_Should_ReturnTrue_When_MatchFound) {
        // Given
        List<int> list = { 1, 2, 3, 4, 5 };

        // When & Then
        EXPECT_TRUE(list.Exists([](int x) { return x > 3; }));
        EXPECT_FALSE(list.Exists([](int x) { return x > 10; }));
    }

    TEST(ListEnhancementTest, Find_Should_ReturnItem_When_MatchFound) {
        // Given
        List<int> list = { 10, 20, 30 };

        // When
        int result = list.Find([](int x) { return x > 15; });

        // Then
        EXPECT_EQ(result, 20);
    }

    TEST(ListEnhancementTest, FindAll_Should_ReturnAllMatchingItems) {
        // Given
        List<int> list = { 1, 2, 3, 4, 5 };

        // When
        auto result = list.FindAll([](int x) { return x % 2 == 0; });

        // Then
        EXPECT_EQ(result.GetCount(), 2);
        EXPECT_EQ(result [0], 2);
        EXPECT_EQ(result [1], 4);
    }

    TEST(ListEnhancementTest, TrueForAll_Should_ReturnCorrectResult) {
        // Given
        List<int> list = { 2, 4, 6 };

        // When & Then
        EXPECT_TRUE(list.TrueForAll([](int x) { return x % 2 == 0; }));
        EXPECT_FALSE(list.TrueForAll([](int x) { return x > 3; }));
    }
}
