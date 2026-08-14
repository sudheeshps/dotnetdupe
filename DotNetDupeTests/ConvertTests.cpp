#include "pch.h"
#include "gtest/gtest.h"
#include "System/Convert.h"
#include "System/Array.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"

using namespace DotNetDupe::System;

namespace DotNetDupeTests {

    // --- Boolean Tests ---

    TEST(ConvertTest, ToBoolean_Given_ValidStrings_When_Converted_Then_ReturnsCorrectBool) {
        // When & Then
        EXPECT_TRUE(Convert::ToBoolean("True"));
        EXPECT_FALSE(Convert::ToBoolean("False"));
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
        // When & Then
        EXPECT_THROW(Convert::ToBoolean("not-a-bool"), FormatException);
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
        // When
        unsigned char result = Convert::ToByte("FF", 16);

        // Then
        EXPECT_EQ(result, 255);
    }

    // --- Int32 Tests ---

    TEST(ConvertTest, ToInt32_Given_ValidString_When_Converted_Then_ReturnsInt) {
        // When
        int result = Convert::ToInt32("12345");

        // Then
        EXPECT_EQ(result, 12345);
    }

    TEST(ConvertTest, ToInt32_Given_InvalidFormat_When_Converted_Then_ThrowsFormatException) {
        // When & Then
        EXPECT_THROW(Convert::ToInt32("abc"), FormatException);
    }

    TEST(ConvertTest, ToInt32_Given_LargeLong_When_Converted_Then_ThrowsOverflowException) {
        // Given
        long long outOfRangeLarge = 2147483648LL;

        // When & Then
        EXPECT_THROW(Convert::ToInt32(outOfRangeLarge), OverflowException);
    }

    // --- Double/Single Tests ---

    TEST(ConvertTest, ToDouble_Given_ValidString_When_Converted_Then_ReturnsDouble) {
        // When
        double result = Convert::ToDouble("123.45");

        // Then
        EXPECT_DOUBLE_EQ(result, 123.45);
    }

    TEST(ConvertTest, ToSingle_Given_ValidString_When_Converted_Then_ReturnsFloat) {
        // When
        float result = Convert::ToSingle("12.34");

        // Then
        EXPECT_FLOAT_EQ(result, 12.34f);
    }

    // --- Char Tests ---

    TEST(ConvertTest, ToChar_Given_ValidString_When_Converted_Then_ReturnsChar) {
        // When
        char result = Convert::ToChar("A");

        // Then
        EXPECT_EQ(result, 'A');
    }

    TEST(ConvertTest, ToChar_Given_InvalidStringLength_When_Converted_Then_ThrowsFormatException) {
        // When & Then
        EXPECT_THROW(Convert::ToChar("ABC"), FormatException);
    }

    // --- ToString Tests ---

    TEST(ConvertTest, ToString_Given_Numeric_When_Converted_Then_ReturnsCorrectString) {
        // Given
        int val = 123;

        // When
        String result = Convert::ToString(val);

        // Then
        EXPECT_TRUE(result == "123");
    }

    TEST(ConvertTest, ToString_Given_Int_When_ConvertedWithBase_Then_ReturnsCorrectString) {
        // Given
        int val = 255;

        // When
        String hex = Convert::ToString(val, 16);

        // Then
        EXPECT_TRUE(hex == "FF");
    }

    TEST(ConvertTest, ToString_Given_InvalidBase_When_Converted_Then_ThrowsArgumentException) {
        // Given
        int val = 255;

        // When & Then
        EXPECT_THROW(Convert::ToString(val, 3), ArgumentException);
    }

    TEST(ConvertTest, NumericConversions_GivenVariousTypes_WhenConverted_ThenReturnsExpectedResults) {
        EXPECT_EQ(Convert::ToSByte(127), 127);
        EXPECT_EQ(Convert::ToSByte("120"), 120);
        EXPECT_EQ(Convert::ToInt16(32000), 32000);
        EXPECT_EQ(Convert::ToInt16("32000"), 32000);
        EXPECT_EQ(Convert::ToInt16("7FFF", 16), 32767);
        EXPECT_EQ(Convert::ToUInt16(65000), 65000);
        EXPECT_EQ(Convert::ToUInt16("65000"), 65000);
        EXPECT_EQ(Convert::ToUInt32(4000000000LL), 4000000000U);
        EXPECT_EQ(Convert::ToUInt32("4000000000"), 4000000000U);
        EXPECT_EQ(Convert::ToInt64(9000000000000LL), 9000000000000LL);
        EXPECT_EQ(Convert::ToInt64("9000000000000"), 9000000000000LL);
        EXPECT_EQ(Convert::ToUInt64("18000000000000000000"), 18000000000000000000ULL);

        EXPECT_STREQ(Convert::ToString(true).GetRawString(), "True");
        EXPECT_STREQ(Convert::ToString(false).GetRawString(), "False");
        EXPECT_STREQ(Convert::ToString(static_cast<unsigned char>(65)).GetRawString(), "65");
        EXPECT_STREQ(Convert::ToString('Z').GetRawString(), "Z");
        EXPECT_STREQ(Convert::ToString(12.5).GetRawString(), "12.5");
        EXPECT_STREQ(Convert::ToString(12.5f).GetRawString(), "12.5");
        EXPECT_STREQ(Convert::ToString(static_cast<short>(42)).GetRawString(), "42");
        EXPECT_STREQ(Convert::ToString(42LL).GetRawString(), "42");
        EXPECT_STREQ(Convert::ToString(static_cast<unsigned short>(42)).GetRawString(), "42");
        EXPECT_STREQ(Convert::ToString(42U).GetRawString(), "42");
        EXPECT_STREQ(Convert::ToString(42ULL).GetRawString(), "42");
        EXPECT_STREQ(Convert::ToString(255LL, 16).GetRawString(), "FF");
        EXPECT_STREQ(Convert::ToString(255LL, 2).GetRawString(), "11111111");
        EXPECT_STREQ(Convert::ToString(255LL, 8).GetRawString(), "377");
        EXPECT_STREQ(Convert::ToString(255LL, 10).GetRawString(), "255");
    }

    TEST(ConvertTest, Base64_GivenByteArray_WhenEncodedAndDecoded_ThenMatchesOriginal) {
        const char rawData[] = "DotNetDupe Base64 Test String!";
        int len = static_cast<int>(std::strlen(rawData));
        Array<char> arrIn(len);
        for (int i = 0; i < len; ++i) arrIn[i] = rawData[i];

        String base64 = Convert::ToBase64String(arrIn);
        EXPECT_FALSE(base64.IsEmpty());

        Array<char> arrOut = Convert::FromBase64String(base64);
        EXPECT_EQ(arrOut.GetLength(), len);
        for (int i = 0; i < len; ++i) {
            EXPECT_EQ(arrOut[i], rawData[i]);
        }
    }
}
