#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Random.h"

using namespace DotNetDupe::System;

namespace SystemTests
{
    namespace RandomTests
    {
        TEST(RandomTest, Next_Should_Return_Non_Negative_Value)
        {
            // Given
            Random random;

            // When
            int result = random.Next();

            // Then
            EXPECT_GE(result, 0);
        }

        TEST(RandomTest, Next_With_MaxValue_Should_Return_Value_In_Range)
        {
            // Given
            Random random;
            int maxValue = 100;

            // When
            int result = random.Next(maxValue);

            // Then
            EXPECT_GE(result, 0);
            EXPECT_LT(result, maxValue);
        }

        TEST(RandomTest, Next_With_Min_And_Max_Value_Should_Return_Value_In_Range)
        {
            // Given
            Random random;
            int minValue = 10;
            int maxValue = 20;

            // When
            int result = random.Next(minValue, maxValue);

            // Then
            EXPECT_GE(result, minValue);
            EXPECT_LT(result, maxValue);
        }

        TEST(RandomTest, NextDouble_Should_Return_Value_In_Range)
        {
            // Given
            Random random;

            // When
            double result = random.NextDouble();

            // Then
            EXPECT_GE(result, 0.0);
            EXPECT_LT(result, 1.0);
        }

        TEST(RandomTest, NextBytes_Should_Fill_Buffer)
        {
            // Given
            Random random;
            unsigned char buffer[10];

            // When
            random.NextBytes(buffer, 10);

            // Then
            // No easy way to check for randomness, but we can check that the buffer is not all zeros.
            bool allZeros = true;
            for (int i = 0; i < 10; ++i)
            {
                if (buffer[i] != 0)
                {
                    allZeros = false;
                    break;
                }
            }
            EXPECT_FALSE(allZeros);
        }
    }
}
