#include "pch.h"
#include "System/Math.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <cmath>

namespace DotNetDupe {
    namespace System {

        double Math::Mean(const Collections::Generic::List<double>& listValues) {
            /// Guard: Ensure list is not empty.
            int iCount = listValues.GetCount();
            if (iCount == 0) throw ArgumentException("Values list cannot be empty.");
            double dSum = 0.0;
            for (int i = 0; i < iCount; ++i) {
                dSum += listValues[i];
            }
            return dSum / static_cast<double>(iCount);
        }

        double Math::Variance(const Collections::Generic::List<double>& listValues, bool bIsSample) {
            /// Guard: Validate sufficient observations.
            int iCount = listValues.GetCount();
            int iMinCount = bIsSample ? 2 : 1;
            if (iCount < iMinCount) throw ArgumentException("Insufficient values to compute variance.");
            double dMean = Mean(listValues);
            double dSumSquares = 0.0;
            for (int i = 0; i < iCount; ++i) {
                double dDiff = listValues[i] - dMean;
                dSumSquares += dDiff * dDiff;
            }
            int iDivisor = bIsSample ? (iCount - 1) : iCount;
            return dSumSquares / static_cast<double>(iDivisor);
        }

        double Math::StandardDeviation(const Collections::Generic::List<double>& listValues, bool bIsSample) {
            /// Return: Square root of variance.
            return std::sqrt(Variance(listValues, bIsSample));
        }

        double Math::StdDev(const Collections::Generic::List<double>& listValues, bool bIsSample) {
            /// Forward: Delegate to StandardDeviation.
            return StandardDeviation(listValues, bIsSample);
        }

        double Math::MovingAverage(const Collections::Generic::List<double>& listValues, int iPeriod) {
            /// Guard: Validate period range and item count.
            if (iPeriod <= 0) throw ArgumentOutOfRangeException("Period must be greater than zero.");
            int iCount = listValues.GetCount();
            if (iCount < iPeriod) throw ArgumentException("Not enough data points for moving average window.");
            double dSum = 0.0;
            int iStart = iCount - iPeriod;
            for (int i = iStart; i < iCount; ++i) {
                dSum += listValues[i];
            }
            return dSum / static_cast<double>(iPeriod);
        }

        Collections::Generic::List<double> Math::MovingAverageSeries(
            const Collections::Generic::List<double>& listValues, int iPeriod) {
            /// Guard: Validate period range.
            if (iPeriod <= 0) throw ArgumentOutOfRangeException("Period must be greater than zero.");
            Collections::Generic::List<double> listResult;
            int iCount = listValues.GetCount();
            if (iCount < iPeriod) return listResult;
            double dCurrentSum = 0.0;
            for (int i = 0; i < iPeriod; ++i) {
                dCurrentSum += listValues[i];
            }
            listResult.Add(dCurrentSum / static_cast<double>(iPeriod));
            for (int i = iPeriod; i < iCount; ++i) {
                dCurrentSum += listValues[i] - listValues[i - iPeriod];
                listResult.Add(dCurrentSum / static_cast<double>(iPeriod));
            }
            return listResult;
        }

        static double ComputeInitialSma(const Collections::Generic::List<double>& listValues, int iPeriod) {
            /// Step: Compute initial simple moving average for EMA seed.
            double dSum = 0.0;
            for (int i = 0; i < iPeriod; ++i) {
                dSum += listValues[i];
            }
            return dSum / static_cast<double>(iPeriod);
        }

        double Math::ExponentialMovingAverage(
            const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing) {
            /// Guard: Validate period and items.
            if (iPeriod <= 0) throw ArgumentOutOfRangeException("Period must be greater than zero.");
            int iCount = listValues.GetCount();
            if (iCount < iPeriod) throw ArgumentException("Not enough data points for EMA.");
            double dMultiplier = dSmoothing / (static_cast<double>(iPeriod) + 1.0);
            double dEma = ComputeInitialSma(listValues, iPeriod);
            for (int i = iPeriod; i < iCount; ++i) {
                dEma = (listValues[i] - dEma) * dMultiplier + dEma;
            }
            return dEma;
        }

        Collections::Generic::List<double> Math::ExponentialMovingAverageSeries(
            const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing) {
            /// Guard: Validate period parameter.
            if (iPeriod <= 0) throw ArgumentOutOfRangeException("Period must be greater than zero.");
            Collections::Generic::List<double> listResult;
            int iCount = listValues.GetCount();
            if (iCount < iPeriod) return listResult;
            double dMultiplier = dSmoothing / (static_cast<double>(iPeriod) + 1.0);
            double dEma = ComputeInitialSma(listValues, iPeriod);
            listResult.Add(dEma);
            for (int i = iPeriod; i < iCount; ++i) {
                dEma = (listValues[i] - dEma) * dMultiplier + dEma;
                listResult.Add(dEma);
            }
            return listResult;
        }

    }
}
