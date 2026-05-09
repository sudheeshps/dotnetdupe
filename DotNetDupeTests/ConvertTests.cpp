#include "pch.h"
#include "gtest/gtest.h"
#include "System/Convert.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"

using namespace DotNetDupe::System;

namespace DotNetDupeTests {

    // --- Boolean Tests ---

    TEST(ConvertTest, ToBoolean_Given_ValidStrings_When_Converted_Then_ReturnsCorrectBool) {
        // Given
        String trueStr(_T("True"));
        String falseStr(_T("False"));

        // When & Then
        EXPECT_TRUE(Convert::ToBoolean(trueStr));
        EXPECT_FALSE(Convert::ToBoolean(falseStr));
    }

    TEST(ConvertTest, ToBoolean_Given_NumericValues_When_Converted_Then_ReturnsCorrectBool) {
        // Given
        int one = 1;
        int zero = 0;

        // When & Then
        EXPECT_TRUE(Convert::ToBoolean(one));
        EXPECT_FALSE(Convert::ToBoolean(zero));
    }

    TEST(ConvertTest, ToBoolean_Given_InvalidString_When_Converted_Then_ThrowsFormatException) {
        // Given
        String invalid(_T("not-a-bool"));

        // When & Then
        EXPECT_THROW(Convert::ToBoolean(invalid), FormatException);
    }

    // --- Byte Tests ---

    TEST(ConvertTest, ToByte_Given_ValidNumeric_When_Converted_Then_ReturnsByte) {
        // Given
        int val = 255;

        // When
        unsigned char result = Convert::ToByte(val);

        // Then
        EXPECT_EQ(result, 255);
    }

    TEST(ConvertTest, ToByte_Given_OutOfRangeNumeric_When_Converted_Then_ThrowsOverflowException) {
        // Given
        int outOfRangeLarge = 256;
        int outOfRangeSmall = -1;

        // When & Then
        EXPECT_THROW(Convert::ToByte(outOfRangeLarge), OverflowException);
        EXPECT_THROW(Convert::ToByte(outOfRangeSmall), OverflowException);
    }

    TEST(ConvertTest, ToByte_Given_StringWithBase_When_Converted_Then_ReturnsCorrectByte) {
        // Given
        String hex(_T("FF"));

        // When
        unsigned char result = Convert::ToByte(hex, 16);

        // Then
        EXPECT_EQ(result, 255);
    }

    // --- Int32 Tests ---

    TEST(ConvertTest, ToInt32_Given_ValidString_When_Converted_Then_ReturnsInt) {
        // Given
        String val(_T("12345"));

        // When
        int result = Convert::ToInt32(val);

        // Then
        EXPECT_EQ(result, 12345);
    }

    TEST(ConvertTest, ToInt32_Given_InvalidFormat_When_Converted_Then_ThrowsFormatException) {
        // Given
        String invalid(_T("abc"));

        // When & Then
        EXPECT_THROW(Convert::ToInt32(invalid), FormatException);
    }

    TEST(ConvertTest, ToInt32_Given_LargeLong_When_Converted_Then_ThrowsOverflowException) {
        // Given
        long long outOfRangeLarge = 2147483648LL;

        // When & Then
        EXPECT_THROW(Convert::ToInt32(outOfRangeLarge), OverflowException);
    }

    // --- Double/Single Tests ---

    TEST(ConvertTest, ToDouble_Given_ValidString_When_Converted_Then_ReturnsDouble) {
        // Given
        String val(_T("123.45"));

        // When
        double result = Convert::ToDouble(val);

        // Then
        EXPECT_DOUBLE_EQ(result, 123.45);
    }

    TEST(ConvertTest, ToSingle_Given_ValidString_When_Converted_Then_ReturnsFloat) {
        // Given
        String val(_T("12.34"));

        // When
        float result = Convert::ToSingle(val);

        // Then
        EXPECT_FLOAT_EQ(result, 12.34f);
    }

    // --- Char Tests ---

    TEST(ConvertTest, ToChar_Given_ValidString_When_Converted_Then_ReturnsChar) {
        // Given
        String val(_T("A"));

        // When
        TCHAR result = Convert::ToChar(val);

        // Then
        EXPECT_EQ(result, _T('A'));
    }

    TEST(ConvertTest, ToChar_Given_InvalidStringLength_When_Converted_Then_ThrowsFormatException) {
        // Given
        String invalid(_T("ABC"));

        // When & Then
        EXPECT_THROW(Convert::ToChar(invalid), FormatException);
    }

    // --- ToString Tests ---

    TEST(ConvertTest, ToString_Given_Numeric_When_Converted_Then_ReturnsCorrectString) {
        // Given
        int val = 123;

        // When
        String result = Convert::ToString(val);

        // Then
        EXPECT_TRUE(result == _T("123"));
    }

    TEST(ConvertTest, ToString_Given_Int_When_ConvertedWithBase_Then_ReturnsCorrectString) {
        // Given
        int val = 255;

        // When
        String hex = Convert::ToString(val, 16);

        // Then
        EXPECT_TRUE(hex == _T("FF"));
    }

    TEST(ConvertTest, ToString_Given_InvalidBase_When_Converted_Then_ThrowsArgumentException) {
        // Given
        int val = 255;

        // When & Then
        EXPECT_THROW(Convert::ToString(val, 3), ArgumentException);
    }
}
