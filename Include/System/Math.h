/// \file Math.h
/// \brief Provides constants and static methods for trigonometric, logarithmic, and other common mathematical functions.
///
/// Standard Citation: IEEE 754 Floating-Point Arithmetic, ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {

        /// \class Math
        /// \brief Provides constants and static methods for trigonometric, logarithmic, and statistical functions.
        ///
        /// \details Implements IEEE 754 standard mathematical primitives and high-order time-series statistics
        /// with zero STL leakage and standard DotNetDupe memory safety guarantees.
        class Math {
        public:
            /// \brief Represents the natural logarithmic base, specified by the constant e.
            static constexpr double E = 2.71828182845904523536;

            /// \brief Represents the ratio of the circumference of a circle to its diameter, specified by the constant pi.
            static constexpr double PI = 3.14159265358979323846;

            /// \brief Represents the number of radians in one turn, specified by the constant tau (2*pi).
            static constexpr double Tau = 6.28318530717958647692;

            /// \brief Returns the absolute value of a 32-bit signed integer.
            DOTNETDUPE_API static int Abs(int iValue);

            /// \brief Returns the absolute value of a 64-bit signed integer.
            DOTNETDUPE_API static long long Abs(long long llValue);

            /// \brief Returns the absolute value of a single-precision floating-point number.
            DOTNETDUPE_API static float Abs(float fValue);

            /// \brief Returns the absolute value of a double-precision floating-point number.
            DOTNETDUPE_API static double Abs(double dValue);

            /// \brief Returns the larger of two 32-bit signed integers.
            DOTNETDUPE_API static int Max(int iVal1, int iVal2);

            /// \brief Returns the larger of two 64-bit signed integers.
            DOTNETDUPE_API static long long Max(long long llVal1, long long llVal2);

            /// \brief Returns the larger of two single-precision floating-point numbers.
            DOTNETDUPE_API static float Max(float fVal1, float fVal2);

            /// \brief Returns the larger of two double-precision floating-point numbers.
            DOTNETDUPE_API static double Max(double dVal1, double dVal2);

            /// \brief Returns the smaller of two 32-bit signed integers.
            DOTNETDUPE_API static int Min(int iVal1, int iVal2);

            /// \brief Returns the smaller of two 64-bit signed integers.
            DOTNETDUPE_API static long long Min(long long llVal1, long long llVal2);

            /// \brief Returns the smaller of two single-precision floating-point numbers.
            DOTNETDUPE_API static float Min(float fVal1, float fVal2);

            /// \brief Returns the smaller of two double-precision floating-point numbers.
            DOTNETDUPE_API static double Min(double dVal1, double dVal2);

            /// \brief Returns a value clamped to the inclusive range of min and max.
            DOTNETDUPE_API static int Clamp(int iValue, int iMin, int iMax);

            /// \brief Returns a value clamped to the inclusive range of min and max.
            DOTNETDUPE_API static long long Clamp(long long llValue, long long llMin, long long llMax);

            /// \brief Returns a value clamped to the inclusive range of min and max.
            DOTNETDUPE_API static double Clamp(double dValue, double dMin, double dMax);

            /// \brief Returns an integer that indicates the sign of a 32-bit signed integer.
            DOTNETDUPE_API static int Sign(int iValue);

            /// \brief Returns an integer that indicates the sign of a 64-bit signed integer.
            DOTNETDUPE_API static int Sign(long long llValue);

            /// \brief Returns an integer that indicates the sign of a double-precision floating-point number.
            DOTNETDUPE_API static int Sign(double dValue);

            /// \brief Returns the largest integral value less than or equal to the specified number.
            DOTNETDUPE_API static double Floor(double dValue);

            /// \brief Returns the smallest integral value that is greater than or equal to the specified number.
            DOTNETDUPE_API static double Ceiling(double dValue);

            /// \brief Rounds a double-precision floating-point value to the nearest integral value.
            DOTNETDUPE_API static double Round(double dValue);

            /// \brief Rounds a double-precision floating-point value to a specified number of fractional digits.
            DOTNETDUPE_API static double Round(double dValue, int iDigits);

            /// \brief Calculates the integral part of a specified double-precision floating-point number.
            DOTNETDUPE_API static double Truncate(double dValue);

            /// \brief Returns the square root of a specified number.
            DOTNETDUPE_API static double Sqrt(double dValue);

            /// \brief Returns the cube root of a specified number.
            DOTNETDUPE_API static double Cbrt(double dValue);

            /// \brief Returns a specified number raised to the specified power.
            DOTNETDUPE_API static double Pow(double dBase, double dExponent);

            /// \brief Returns e raised to the specified power.
            DOTNETDUPE_API static double Exp(double dValue);

            /// \brief Returns the natural (base e) logarithm of a specified number.
            DOTNETDUPE_API static double Log(double dValue);

            /// \brief Returns the logarithm of a specified number in a specified base.
            DOTNETDUPE_API static double Log(double dValue, double dNewBase);

            /// \brief Returns the base 10 logarithm of a specified number.
            DOTNETDUPE_API static double Log10(double dValue);

            /// \brief Returns the base 2 logarithm of a specified number.
            DOTNETDUPE_API static double Log2(double dValue);

            /// \brief Returns the sine of the specified angle.
            DOTNETDUPE_API static double Sin(double dValue);

            /// \brief Returns the cosine of the specified angle.
            DOTNETDUPE_API static double Cos(double dValue);

            /// \brief Returns the tangent of the specified angle.
            DOTNETDUPE_API static double Tan(double dValue);

            /// \brief Returns the angle whose sine is the specified number.
            DOTNETDUPE_API static double Asin(double dValue);

            /// \brief Returns the angle whose cosine is the specified number.
            DOTNETDUPE_API static double Acos(double dValue);

            /// \brief Returns the angle whose tangent is the specified number.
            DOTNETDUPE_API static double Atan(double dValue);

            /// \brief Returns the angle whose tangent is the quotient of two specified numbers.
            DOTNETDUPE_API static double Atan2(double dY, double dX);

            /// \brief Returns the hyperbolic sine of the specified angle.
            DOTNETDUPE_API static double Sinh(double dValue);

            /// \brief Returns the hyperbolic cosine of the specified angle.
            DOTNETDUPE_API static double Cosh(double dValue);

            /// \brief Returns the hyperbolic tangent of the specified angle.
            DOTNETDUPE_API static double Tanh(double dValue);

            /// \brief Computes the arithmetic mean of a sequence of double values.
            DOTNETDUPE_API static double Mean(const Collections::Generic::List<double>& listValues);

            /// \brief Computes the variance of a sequence of double values.
            DOTNETDUPE_API static double Variance(const Collections::Generic::List<double>& listValues, bool bIsSample = false);

            /// \brief Computes the standard deviation of a sequence of double values.
            DOTNETDUPE_API static double StandardDeviation(const Collections::Generic::List<double>& listValues, bool bIsSample = false);

            /// \brief Computes the standard deviation of a sequence of double values (abbreviated alias).
            DOTNETDUPE_API static double StdDev(const Collections::Generic::List<double>& listValues, bool bIsSample = false);

            /// \brief Computes the simple moving average (SMA) over the final window of length iPeriod.
            DOTNETDUPE_API static double MovingAverage(const Collections::Generic::List<double>& listValues, int iPeriod);

            /// \brief Computes the rolling simple moving average (SMA) series across all values.
            DOTNETDUPE_API static Collections::Generic::List<double> MovingAverageSeries(const Collections::Generic::List<double>& listValues, int iPeriod);

            /// \brief Computes the exponential moving average (EMA) value for the sequence.
            DOTNETDUPE_API static double ExponentialMovingAverage(const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing = 2.0);

            /// \brief Computes the rolling exponential moving average (EMA) series across the sequence.
            DOTNETDUPE_API static Collections::Generic::List<double> ExponentialMovingAverageSeries(const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing = 2.0);
        };

    }
}
