#include "pch.h"
#include "System/Numerics/Statistics.h"
#include "System/Math.h"

namespace DotNetDupe {
    namespace System {
        namespace Numerics {

            double Statistics::Mean(const Collections::Generic::List<double>& listValues) {
                /// Forward: Delegate to Math::Mean.
                return Math::Mean(listValues);
            }

            double Statistics::Variance(const Collections::Generic::List<double>& listValues, bool bIsSample) {
                /// Forward: Delegate to Math::Variance.
                return Math::Variance(listValues, bIsSample);
            }

            double Statistics::StandardDeviation(const Collections::Generic::List<double>& listValues, bool bIsSample) {
                /// Forward: Delegate to Math::StandardDeviation.
                return Math::StandardDeviation(listValues, bIsSample);
            }

            double Statistics::StdDev(const Collections::Generic::List<double>& listValues, bool bIsSample) {
                /// Forward: Delegate to Math::StdDev.
                return Math::StdDev(listValues, bIsSample);
            }

            double Statistics::MovingAverage(const Collections::Generic::List<double>& listValues, int iPeriod) {
                /// Forward: Delegate to Math::MovingAverage.
                return Math::MovingAverage(listValues, iPeriod);
            }

            Collections::Generic::List<double> Statistics::MovingAverageSeries(
                const Collections::Generic::List<double>& listValues, int iPeriod) {
                /// Forward: Delegate to Math::MovingAverageSeries.
                return Math::MovingAverageSeries(listValues, iPeriod);
            }

            double Statistics::ExponentialMovingAverage(
                const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing) {
                /// Forward: Delegate to Math::ExponentialMovingAverage.
                return Math::ExponentialMovingAverage(listValues, iPeriod, dSmoothing);
            }

            Collections::Generic::List<double> Statistics::ExponentialMovingAverageSeries(
                const Collections::Generic::List<double>& listValues, int iPeriod, double dSmoothing) {
                /// Forward: Delegate to Math::ExponentialMovingAverageSeries.
                return Math::ExponentialMovingAverageSeries(listValues, iPeriod, dSmoothing);
            }

        }
    }
}
