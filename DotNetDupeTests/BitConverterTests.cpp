#include "pch.h"
#include "gtest/gtest.h"
#include "System/BitConverter.h"
#include "System/Array.h"
#include "System/String.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <limits>

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace BitConverterTests {
        // Positive Cases

        TEST(BitConverterTest, Given_BooleanTrue_When_GetBytesCalled_Then_ReturnsArrayWithOne) {
            // Given
            bool value = true;

            // When
            Array<byte> bytes = BitConverter::GetBytes(value);

            // Then
            EXPECT_EQ(1, bytes.GetLength());
            EXPECT_EQ(1, bytes [0]);
        }

        TEST(BitConverterTest, Given_Int32Value_When_GetBytesCalled_Then_ReturnsCorrectByteArray) {
            // Given
            int value = 0x01020304;

            // When
            Array<byte> bytes = BitConverter::GetBytes(value);

            // Then
            EXPECT_EQ(4, bytes.GetLength());
            if (BitConverter::IsLittleEndian) {
                EXPECT_EQ(0x04, bytes [0]);
                EXPECT_EQ(0x03, bytes [1]);
                EXPECT_EQ(0x02, bytes [2]);
                EXPECT_EQ(0x01, bytes [3]);
            }
        }

        TEST(BitConverterTest, Given_ByteArray_When_ToInt32Called_Then_ReturnsCorrectValue) {
            // Given
            Array<byte> bytes = { 0x04, 0x03, 0x02, 0x01 };

            // When
            int result = BitConverter::ToInt32(bytes, 0);

            // Then
            if (BitConverter::IsLittleEndian) {
                EXPECT_EQ(0x01020304, result);
            }
        }

        TEST(BitConverterTest, Given_ByteArray_When_ToStringCalled_Then_ReturnsCorrectHexFormattedString) {
            // Given
            Array<byte> bytes = { 0x0A, 0x1B, 0xFF };

            // When
            String result = BitConverter::ToString(bytes);

            // Then
            EXPECT_EQ(String("0A-1B-FF"), result);
        }

        TEST(BitConverterTest, Given_DoubleValue_When_DoubleToInt64BitsCalled_Then_ReturnsCorrectBits) {
            // Given
            double value = 1.0;

            // When
            long long bits = BitConverter::DoubleToInt64Bits(value);

            // Then
            EXPECT_EQ(0x3FF0000000000000LL, bits);
        }

        TEST(BitConverterTest, Given_Bits_When_Int64BitsToDoubleCalled_Then_ReturnsCorrectDouble) {
            // Given
            long long bits = 0x3FF0000000000000LL;

            // When
            double value = BitConverter::Int64BitsToDouble(bits);

            // Then
            EXPECT_EQ(1.0, value);
        }

        // Negative Cases

        TEST(BitConverterTest, Given_EmptyArray_When_ToInt32Called_Then_ThrowsArgumentNullException) {
            // Given
            Array<byte> bytes(0);

            // When/Then
            EXPECT_THROW(BitConverter::ToInt32(bytes, 0), ArgumentNullException);
        }

        TEST(BitConverterTest, Given_InvalidStartIndex_When_ToInt32Called_Then_ThrowsArgumentOutOfRangeException) {
            // Given
            Array<byte> bytes = { 1, 2, 3 };

            // When/Then
            EXPECT_THROW(BitConverter::ToInt32(bytes, 1), ArgumentOutOfRangeException); // Needs 4 bytes
            EXPECT_THROW(BitConverter::ToInt32(bytes, -1), ArgumentOutOfRangeException);
            EXPECT_THROW(BitConverter::ToInt32(bytes, 4), ArgumentOutOfRangeException);
        }

        TEST(BitConverterTest, Given_ShortByteArray_When_ToStringCalledWithTooLargeLength_Then_ThrowsArgumentException) {
            // Given
            Array<byte> bytes = { 1, 2, 3 };

            // When/Then
            EXPECT_THROW(BitConverter::ToString(bytes, 0, 4), ArgumentException);
        }

        // Corner Cases

        TEST(BitConverterTest, Given_MinValue_When_GetBytesCalled_Then_ReturnsCorrectBytes) {
            // Given
            int value = (std::numeric_limits<int>::min)();

            // When
            Array<byte> bytes = BitConverter::GetBytes(value);

            // Then
            if (BitConverter::IsLittleEndian) {
                EXPECT_EQ(0x00, bytes [0]);
                EXPECT_EQ(0x00, bytes [1]);
                EXPECT_EQ(0x00, bytes [2]);
                EXPECT_EQ(0x80, bytes [3]);
            }
        }

        TEST(BitConverterTest, Given_MaxValue_When_GetBytesCalled_Then_ReturnsCorrectBytes) {
            // Given
            int value = (std::numeric_limits<int>::max)();

            // When
            Array<byte> bytes = BitConverter::GetBytes(value);

            // Then
            if (BitConverter::IsLittleEndian) {
                EXPECT_EQ(0xFF, bytes [0]);
                EXPECT_EQ(0xFF, bytes [1]);
                EXPECT_EQ(0xFF, bytes [2]);
                EXPECT_EQ(0x7F, bytes [3]);
            }
        }

        TEST(BitConverterTest, Given_AllPrimitiveTypes_When_GetBytesAndConvertedBack_Then_Matches) {
            char c = 'A';
            Array<byte> bChar = BitConverter::GetBytes(c);
            EXPECT_EQ('A', BitConverter::ToChar(bChar, 0));

            short s = 1234;
            Array<byte> bShort = BitConverter::GetBytes(s);
            EXPECT_EQ(1234, BitConverter::ToInt16(bShort, 0));

            unsigned short us = 5678;
            Array<byte> bUShort = BitConverter::GetBytes(us);
            EXPECT_EQ(5678, BitConverter::ToUInt16(bUShort, 0));

            unsigned int ui = 99999;
            Array<byte> bUInt = BitConverter::GetBytes(ui);
            EXPECT_EQ(99999u, BitConverter::ToUInt32(bUInt, 0));

            long long ll = 123456789012345LL;
            Array<byte> bLL = BitConverter::GetBytes(ll);
            EXPECT_EQ(123456789012345LL, BitConverter::ToInt64(bLL, 0));

            unsigned long long ull = 987654321098765ULL;
            Array<byte> bULL = BitConverter::GetBytes(ull);
            EXPECT_EQ(987654321098765ULL, BitConverter::ToUInt64(bULL, 0));

            float f = 3.14f;
            Array<byte> bFloat = BitConverter::GetBytes(f);
            EXPECT_FLOAT_EQ(3.14f, BitConverter::ToSingle(bFloat, 0));

            double d = 2.718281828;
            Array<byte> bDouble = BitConverter::GetBytes(d);
            EXPECT_DOUBLE_EQ(2.718281828, BitConverter::ToDouble(bDouble, 0));

            bool b = false;
            Array<byte> bBool = BitConverter::GetBytes(b);
            EXPECT_FALSE(BitConverter::ToBoolean(bBool, 0));
        }

        TEST(BitConverterTest, Given_SingleFloat_When_SingleToInt32BitsCalled_Then_ReturnsBits) {
            float f = 1.0f;
            int bits = BitConverter::SingleToInt32Bits(f);
            float f2 = BitConverter::Int32BitsToSingle(bits);
            EXPECT_FLOAT_EQ(f, f2);
        }
    }
}
