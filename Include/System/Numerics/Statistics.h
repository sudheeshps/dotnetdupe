/// \file Statistics.h
/// \brief Provides high-order statistical methods and moving window functions for numeric collections.
///
/// Standard Citation: IEEE 754 Floating-Point Arithmetic.

#pragma once

#include "Common.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Numerics {

            /// \class Statistics
            /// \brief Provides static methods for mean, variance, standard deviation, and moving averages.
            ///
            /// \details Encapsulates high-order statistical and financial moving-window analysis
            /// compliant with standard DotNetDupe zero STL leakage constraints.
            class Statistics {
            public:
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
}
