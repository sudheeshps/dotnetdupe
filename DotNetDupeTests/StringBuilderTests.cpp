#include "pch.h"
#include "gtest/gtest.h"
#include "System/Text/StringBuilder.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

namespace DotNetDupeTests {

    TEST(StringBuilderTest, Append_Given_MultipleTypes_When_Called_Then_AccumulatesCorrectly) {
        // Given
        StringBuilder sb;

        // When
        sb.Append(_T("Hello "))
          .Append(123)
          .Append(_T(" "))
          .Append(true)
          .Append(_T(" "))
          .Append(3.14);

        // Then
        EXPECT_TRUE(sb.ToString() == _T("Hello 123 True 3.140000"));
    }

    TEST(StringBuilderTest, AppendLine_Given_String_When_Called_Then_AddsNewline) {
        // Given
        StringBuilder sb;

        // When
        sb.AppendLine(_T("Line 1"));
        sb.AppendLine(_T("Line 2"));

        // Then
        EXPECT_TRUE(sb.ToString() == _T("Line 1\r\nLine 2\r\n"));
    }

    TEST(StringBuilderTest, Clear_When_Called_Then_EmptiesBuffer) {
        // Given
        StringBuilder sb(_T("Initial"));

        // When
        sb.Clear();

        // Then
        EXPECT_EQ(sb.GetLength(), 0);
        EXPECT_TRUE(sb.ToString() == _T(""));
    }

    TEST(StringBuilderTest, Capacity_When_Set_Then_ReservesMemory) {
        // Given
        StringBuilder sb;

        // When
        sb.SetCapacity(100);

        // Then
        EXPECT_GE(sb.GetCapacity(), 100);
    }
}
