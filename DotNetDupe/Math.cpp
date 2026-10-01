#include "pch.h"
#include "System/Math.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <cmath>

namespace DotNetDupe {
    namespace System {

        int Math::Abs(int iValue) {
            /// Return: Absolute value of 32-bit signed integer.
            return std::abs(iValue);
        }

        long long Math::Abs(long long llValue) {
            /// Return: Absolute value of 64-bit signed integer.
            return std::abs(llValue);
        }

        float Math::Abs(float fValue) {
            /// Return: Absolute value of single-precision float.
            return std::fabs(fValue);
        }

        double Math::Abs(double dValue) {
            /// Return: Absolute value of double-precision float.
            return std::fabs(dValue);
        }

        int Math::Max(int iVal1, int iVal2) {
            /// Return: Maximum of two 32-bit integers.
            return (iVal1 >= iVal2) ? iVal1 : iVal2;
        }

        long long Math::Max(long long llVal1, long long llVal2) {
            /// Return: Maximum of two 64-bit integers.
            return (llVal1 >= llVal2) ? llVal1 : llVal2;
        }

        float Math::Max(float fVal1, float fVal2) {
            /// Return: Maximum of two single-precision floats.
            return (fVal1 >= fVal2) ? fVal1 : fVal2;
        }

        double Math::Max(double dVal1, double dVal2) {
            /// Return: Maximum of two double-precision floats.
            return (dVal1 >= dVal2) ? dVal1 : dVal2;
        }

        int Math::Min(int iVal1, int iVal2) {
            /// Return: Minimum of two 32-bit integers.
            return (iVal1 <= iVal2) ? iVal1 : iVal2;
        }

        long long Math::Min(long long llVal1, long long llVal2) {
            /// Return: Minimum of two 64-bit integers.
            return (llVal1 <= llVal2) ? llVal1 : llVal2;
        }

        float Math::Min(float fVal1, float fVal2) {
            /// Return: Minimum of two single-precision floats.
            return (fVal1 <= fVal2) ? fVal1 : fVal2;
        }

        double Math::Min(double dVal1, double dVal2) {
            /// Return: Minimum of two double-precision floats.
            return (dVal1 <= dVal2) ? dVal1 : dVal2;
        }

        int Math::Clamp(int iValue, int iMin, int iMax) {
            /// Guard: Validate range boundaries.
            if (iMin > iMax) throw ArgumentException("min cannot be greater than max.");
            if (iValue < iMin) return iMin;
            if (iValue > iMax) return iMax;
            return iValue;
        }

        long long Math::Clamp(long long llValue, long long llMin, long long llMax) {
            /// Guard: Validate range boundaries.
            if (llMin > llMax) throw ArgumentException("min cannot be greater than max.");
            if (llValue < llMin) return llMin;
            if (llValue > llMax) return llMax;
            return llValue;
        }

        double Math::Clamp(double dValue, double dMin, double dMax) {
            /// Guard: Validate range boundaries.
            if (dMin > dMax) throw ArgumentException("min cannot be greater than max.");
            if (dValue < dMin) return dMin;
            if (dValue > dMax) return dMax;
            return dValue;
        }

        int Math::Sign(int iValue) {
            /// Return: Sign of 32-bit integer.
            if (iValue > 0) return 1;
            if (iValue < 0) return -1;
            return 0;
        }

        int Math::Sign(long long llValue) {
            /// Return: Sign of 64-bit integer.
            if (llValue > 0) return 1;
            if (llValue < 0) return -1;
            return 0;
        }

        int Math::Sign(double dValue) {
            /// Return: Sign of double-precision float.
            if (dValue > 0.0) return 1;
            if (dValue < 0.0) return -1;
            return 0;
        }

        double Math::Floor(double dValue) {
            /// Return: Largest integer less than or equal to value.
            return std::floor(dValue);
        }

        double Math::Ceiling(double dValue) {
            /// Return: Smallest integer greater than or equal to value.
            return std::ceil(dValue);
        }

        double Math::Round(double dValue) {
            /// Return: Round to nearest integral value.
            return std::round(dValue);
        }

        double Math::Round(double dValue, int iDigits) {
            /// Guard: Validate digits parameter.
            if (iDigits < 0 || iDigits > 15) throw ArgumentOutOfRangeException("Digits must be between 0 and 15.");
            double dFactor = std::pow(10.0, iDigits);
            return std::round(dValue * dFactor) / dFactor;
        }

        double Math::Truncate(double dValue) {
            /// Return: Integral part of value.
            return std::trunc(dValue);
        }

        double Math::Sqrt(double dValue) {
            /// Return: Square root of value.
            return std::sqrt(dValue);
        }

        double Math::Cbrt(double dValue) {
            /// Return: Cube root of value.
            return std::cbrt(dValue);
        }

        double Math::Pow(double dBase, double dExponent) {
            /// Return: Value raised to exponent.
            return std::pow(dBase, dExponent);
        }

        double Math::Exp(double dValue) {
            /// Return: e raised to power.
            return std::exp(dValue);
        }

        double Math::Log(double dValue) {
            /// Return: Natural logarithm.
            return std::log(dValue);
        }

        double Math::Log(double dValue, double dNewBase) {
            /// Guard: Validate logarithm base.
            if (dNewBase <= 0.0 || dNewBase == 1.0) throw ArgumentException("Base must be positive and not equal to 1.");
            return std::log(dValue) / std::log(dNewBase);
        }

        double Math::Log10(double dValue) {
            /// Return: Base 10 logarithm.
            return std::log10(dValue);
        }

        double Math::Log2(double dValue) {
            /// Return: Base 2 logarithm.
            return std::log2(dValue);
        }

        double Math::Sin(double dValue) {
            /// Return: Sine of angle.
            return std::sin(dValue);
        }

        double Math::Cos(double dValue) {
            /// Return: Cosine of angle.
            return std::cos(dValue);
        }

        double Math::Tan(double dValue) {
            /// Return: Tangent of angle.
            return std::tan(dValue);
        }

        double Math::Asin(double dValue) {
            /// Return: Arc sine.
            return std::asin(dValue);
        }

        double Math::Acos(double dValue) {
            /// Return: Arc cosine.
            return std::acos(dValue);
        }

        double Math::Atan(double dValue) {
            /// Return: Arc tangent.
            return std::atan(dValue);
        }

        double Math::Atan2(double dY, double dX) {
            /// Return: Four-quadrant arc tangent.
            return std::atan2(dY, dX);
        }

        double Math::Sinh(double dValue) {
            /// Return: Hyperbolic sine.
            return std::sinh(dValue);
        }

        double Math::Cosh(double dValue) {
            /// Return: Hyperbolic cosine.
            return std::cosh(dValue);
        }

        double Math::Tanh(double dValue) {
            /// Return: Hyperbolic tangent.
            return std::tanh(dValue);
        }

    }
}
