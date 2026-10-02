#include "pch.h"
#include "gtest/gtest.h"
#include "System/Math.h"
#include "System/Numerics/Statistics.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/Collections/Generic/List.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Numerics;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    TEST(MathTests, GivenPositiveAndNegativeIntegers_WhenAbsCalled_ThenReturnsAbsoluteValues) {
        // Given & When & Then
        EXPECT_EQ(Math::Abs(10), 10);
        EXPECT_EQ(Math::Abs(-10), 10);
        EXPECT_EQ(Math::Abs(0), 0);
        EXPECT_EQ(Math::Abs(100LL), 100LL);
        EXPECT_EQ(Math::Abs(-100LL), 100LL);
        EXPECT_FLOAT_EQ(Math::Abs(3.5f), 3.5f);
        EXPECT_FLOAT_EQ(Math::Abs(-3.5f), 3.5f);
        EXPECT_DOUBLE_EQ(Math::Abs(42.5), 42.5);
        EXPECT_DOUBLE_EQ(Math::Abs(-42.5), 42.5);
    }

    TEST(MathTests, GivenValues_WhenMaxMinCalled_ThenReturnsCorrectExtremes) {
        // Given & When & Then
        EXPECT_EQ(Math::Max(10, 20), 20);
        EXPECT_EQ(Math::Min(10, 20), 10);
        EXPECT_EQ(Math::Max(50LL, -20LL), 50LL);
        EXPECT_EQ(Math::Min(50LL, -20LL), -20LL);
        EXPECT_FLOAT_EQ(Math::Max(1.5f, 2.5f), 2.5f);
        EXPECT_FLOAT_EQ(Math::Min(1.5f, 2.5f), 1.5f);
        EXPECT_DOUBLE_EQ(Math::Max(3.14, 2.71), 3.14);
        EXPECT_DOUBLE_EQ(Math::Min(3.14, 2.71), 2.71);
    }

    TEST(MathTests, GivenValueAndRange_WhenClamped_ThenReturnsClampedValue) {
        // Given & When & Then
        EXPECT_EQ(Math::Clamp(5, 1, 10), 5);
        EXPECT_EQ(Math::Clamp(-5, 1, 10), 1);
        EXPECT_EQ(Math::Clamp(15, 1, 10), 10);

        EXPECT_EQ(Math::Clamp(5LL, 0LL, 10LL), 5LL);
        EXPECT_DOUBLE_EQ(Math::Clamp(12.5, 0.0, 10.0), 10.0);
        EXPECT_DOUBLE_EQ(Math::Clamp(-1.5, 0.0, 10.0), 0.0);
    }

    TEST(MathTests, GivenInvertedRange_WhenClamped_ThenThrowsArgumentException) {
        // Given & When & Then
        EXPECT_THROW(Math::Clamp(5, 10, 1), ArgumentException);
        EXPECT_THROW(Math::Clamp(5LL, 10LL, 1LL), ArgumentException);
        EXPECT_THROW(Math::Clamp(5.0, 10.0, 1.0), ArgumentException);
    }

    TEST(MathTests, GivenValues_WhenSignCalled_ThenReturnsCorrectSign) {
        // Given & When & Then
        EXPECT_EQ(Math::Sign(10), 1);
        EXPECT_EQ(Math::Sign(-10), -1);
        EXPECT_EQ(Math::Sign(0), 0);

        EXPECT_EQ(Math::Sign(10LL), 1);
        EXPECT_EQ(Math::Sign(-10LL), -1);
        EXPECT_EQ(Math::Sign(0LL), 0);

        EXPECT_EQ(Math::Sign(3.14), 1);
        EXPECT_EQ(Math::Sign(-3.14), -1);
        EXPECT_EQ(Math::Sign(0.0), 0);
    }

    TEST(MathTests, GivenFloatingPoint_WhenFloorCeilingRoundTruncateCalled_ThenReturnsExpected) {
        // Given & When & Then
        EXPECT_DOUBLE_EQ(Math::Floor(3.7), 3.0);
        EXPECT_DOUBLE_EQ(Math::Floor(-3.7), -4.0);

        EXPECT_DOUBLE_EQ(Math::Ceiling(3.2), 4.0);
        EXPECT_DOUBLE_EQ(Math::Ceiling(-3.2), -3.0);

        EXPECT_DOUBLE_EQ(Math::Round(3.6), 4.0);
        EXPECT_DOUBLE_EQ(Math::Round(3.2), 3.0);

        EXPECT_DOUBLE_EQ(Math::Round(3.14159, 2), 3.14);
        EXPECT_DOUBLE_EQ(Math::Round(3.14159, 4), 3.1416);

        EXPECT_DOUBLE_EQ(Math::Truncate(3.9), 3.0);
        EXPECT_DOUBLE_EQ(Math::Truncate(-3.9), -3.0);
    }

    TEST(MathTests, GivenNegativeDigits_WhenRoundWithDigitsCalled_ThenThrowsArgumentOutOfRangeException) {
        // Given & When & Then
        EXPECT_THROW(Math::Round(3.14, -1), ArgumentOutOfRangeException);
        EXPECT_THROW(Math::Round(3.14, 16), ArgumentOutOfRangeException);
    }

    TEST(MathTests, GivenNumbers_WhenPowersAndRootsCalled_ThenComputesCorrectly) {
        // Given & When & Then
        EXPECT_DOUBLE_EQ(Math::Sqrt(16.0), 4.0);
        EXPECT_DOUBLE_EQ(Math::Cbrt(27.0), 3.0);
        EXPECT_DOUBLE_EQ(Math::Pow(2.0, 3.0), 8.0);
        EXPECT_NEAR(Math::Exp(1.0), Math::E, 0.000001);
        EXPECT_NEAR(Math::Log(Math::E), 1.0, 0.000001);
        EXPECT_NEAR(Math::Log(8.0, 2.0), 3.0, 0.000001);
        EXPECT_DOUBLE_EQ(Math::Log10(100.0), 2.0);
        EXPECT_DOUBLE_EQ(Math::Log2(8.0), 3.0);
    }

    TEST(MathTests, GivenInvalidLogBase_WhenLogCalled_ThenThrowsArgumentException) {
        // Given & When & Then
        EXPECT_THROW(Math::Log(10.0, 0.0), ArgumentException);
        EXPECT_THROW(Math::Log(10.0, 1.0), ArgumentException);
        EXPECT_THROW(Math::Log(10.0, -2.0), ArgumentException);
    }

    TEST(MathTests, GivenTrigonometricAngles_WhenEvaluated_ThenComputesExpectedResults) {
        // Given & When & Then
        EXPECT_NEAR(Math::Sin(0.0), 0.0, 0.000001);
        EXPECT_NEAR(Math::Sin(Math::PI / 2.0), 1.0, 0.000001);
        EXPECT_NEAR(Math::Cos(0.0), 1.0, 0.000001);
        EXPECT_NEAR(Math::Cos(Math::PI), -1.0, 0.000001);
        EXPECT_NEAR(Math::Tan(0.0), 0.0, 0.000001);

        EXPECT_NEAR(Math::Asin(0.0), 0.0, 0.000001);
        EXPECT_NEAR(Math::Acos(1.0), 0.0, 0.000001);
        EXPECT_NEAR(Math::Atan(0.0), 0.0, 0.000001);
        EXPECT_NEAR(Math::Atan2(1.0, 1.0), Math::PI / 4.0, 0.000001);

        EXPECT_NEAR(Math::Sinh(0.0), 0.0, 0.000001);
        EXPECT_NEAR(Math::Cosh(0.0), 1.0, 0.000001);
        EXPECT_NEAR(Math::Tanh(0.0), 0.0, 0.000001);
    }

    TEST(MathTests, GivenList_WhenMeanCalculated_ThenReturnsCorrectAverage) {
        // Given
        List<double> values = { 10.0, 20.0, 30.0, 40.0, 50.0 };

        // When
        double mean = Math::Mean(values);

        // Then
        EXPECT_DOUBLE_EQ(mean, 30.0);
    }

    TEST(MathTests, GivenEmptyList_WhenMeanCalculated_ThenThrowsArgumentException) {
        // Given
        List<double> emptyList;

        // When & Then
        EXPECT_THROW(Math::Mean(emptyList), ArgumentException);
    }

    TEST(MathTests, GivenList_WhenVarianceAndStandardDeviationCalculated_ThenComputesAccurately) {
        // Given
        List<double> values = { 2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0 };

        // When
        double popVar = Math::Variance(values, false);
        double sampleVar = Math::Variance(values, true);
        double popStd = Math::StandardDeviation(values, false);
        double sampleStd = Math::StdDev(values, true);

        // Then
        EXPECT_DOUBLE_EQ(popVar, 4.0);
        EXPECT_NEAR(sampleVar, 4.571428, 0.0001);
        EXPECT_DOUBLE_EQ(popStd, 2.0);
        EXPECT_NEAR(sampleStd, 2.138089, 0.0001);
    }

    TEST(MathTests, GivenInsufficientData_WhenVarianceCalculated_ThenThrowsArgumentException) {
        // Given
        List<double> singleValue = { 10.0 };
        List<double> emptyList;

        // When & Then
        EXPECT_THROW(Math::Variance(singleValue, true), ArgumentException);
        EXPECT_THROW(Math::Variance(emptyList, false), ArgumentException);
    }

    TEST(MathTests, GivenPrices_WhenMovingAverageCalculated_ThenReturnsWindowAverage) {
        // Given
        List<double> prices = { 10.0, 20.0, 30.0, 40.0, 50.0 };

        // When
        double sma3 = Math::MovingAverage(prices, 3);
        double sma5 = Math::MovingAverage(prices, 5);

        // Then
        // Last 3 values: 30, 40, 50 -> average = 40.0
        EXPECT_DOUBLE_EQ(sma3, 40.0);
        // All 5 values: 10, 20, 30, 40, 50 -> average = 30.0
        EXPECT_DOUBLE_EQ(sma5, 30.0);
    }

    TEST(MathTests, GivenInvalidMovingAverageParams_WhenMovingAverageCalculated_ThenThrowsException) {
        // Given
        List<double> prices = { 10.0, 20.0 };

        // When & Then
        EXPECT_THROW(Math::MovingAverage(prices, 0), ArgumentOutOfRangeException);
        EXPECT_THROW(Math::MovingAverage(prices, -2), ArgumentOutOfRangeException);
        EXPECT_THROW(Math::MovingAverage(prices, 3), ArgumentException);
    }

    TEST(MathTests, GivenPrices_WhenMovingAverageSeriesCalculated_ThenReturnsRollingWindowAverages) {
        // Given
        List<double> prices = { 10.0, 20.0, 30.0, 40.0, 50.0 };

        // When
        List<double> series = Math::MovingAverageSeries(prices, 3);

        // Then
        // Window 1: 10, 20, 30 -> 20.0
        // Window 2: 20, 30, 40 -> 30.0
        // Window 3: 30, 40, 50 -> 40.0
        EXPECT_EQ(series.GetCount(), 3);
        EXPECT_DOUBLE_EQ(series[0], 20.0);
        EXPECT_DOUBLE_EQ(series[1], 30.0);
        EXPECT_DOUBLE_EQ(series[2], 40.0);
    }

    TEST(MathTests, GivenPrices_WhenExponentialMovingAverageCalculated_ThenMatchesExpectedEma) {
        // Given
        List<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };

        // When
        double ema = Math::ExponentialMovingAverage(prices, 3);
        List<double> emaSeries = Math::ExponentialMovingAverageSeries(prices, 3);

        // Then
        // Seed SMA (first 3): (10 + 11 + 12)/3 = 11.0
        // Multiplier k = 2 / (3 + 1) = 0.5
        // Price 4 (13): (13 - 11) * 0.5 + 11 = 12.0
        // Price 5 (14): (14 - 12) * 0.5 + 12 = 13.0
        EXPECT_DOUBLE_EQ(ema, 13.0);
        EXPECT_EQ(emaSeries.GetCount(), 3);
        EXPECT_DOUBLE_EQ(emaSeries[0], 11.0);
        EXPECT_DOUBLE_EQ(emaSeries[1], 12.0);
        EXPECT_DOUBLE_EQ(emaSeries[2], 13.0);
    }

    TEST(MathTests, GivenStatisticsClass_WhenMethodsCalled_ThenProducesIdenticalResultsToMath) {
        // Given
        List<double> prices = { 10.0, 20.0, 30.0, 40.0, 50.0 };

        // When & Then
        EXPECT_DOUBLE_EQ(Statistics::Mean(prices), Math::Mean(prices));
        EXPECT_DOUBLE_EQ(Statistics::Variance(prices, true), Math::Variance(prices, true));
        EXPECT_DOUBLE_EQ(Statistics::StandardDeviation(prices), Math::StandardDeviation(prices));
        EXPECT_DOUBLE_EQ(Statistics::StdDev(prices), Math::StdDev(prices));
        EXPECT_DOUBLE_EQ(Statistics::MovingAverage(prices, 3), Math::MovingAverage(prices, 3));
        EXPECT_DOUBLE_EQ(Statistics::ExponentialMovingAverage(prices, 3), Math::ExponentialMovingAverage(prices, 3));
    }

}
