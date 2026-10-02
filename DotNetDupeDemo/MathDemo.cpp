#include "System/Math.h"
#include "System/Numerics/Statistics.h"
#include "System/Collections/Generic/List.h"
#include "System/Console.h"
#include "System/Convert.h"
#include "System/String.h"
#include "Demos.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Numerics;
using namespace DotNetDupe::System::Collections::Generic;

void DemonstrateMath() {
    Console::WriteLine("\n--- System::Math & System::Numerics::Statistics Demonstration ---");

    // 1. Basic Arithmetic and Transcendental Primitives
    double dValue = -42.75;
    Console::WriteLine(String::Format("Abs({0}) = {1}", dValue, Math::Abs(dValue)));
    Console::WriteLine(String::Format("Floor(3.7) = {0}, Ceiling(3.2) = {1}, Round(3.14159, 2) = {2}",
        Math::Floor(3.7), Math::Ceiling(3.2), Math::Round(3.14159, 2)));
    Console::WriteLine(String::Format("Sqrt(16) = {0}, Pow(2, 8) = {1}, Log10(1000) = {2}",
        Math::Sqrt(16.0), Math::Pow(2.0, 8.0), Math::Log10(1000.0)));
    Console::WriteLine(String::Format("Sin(PI/2) = {0}, Cos(0) = {1}",
        Math::Sin(Math::PI / 2.0), Math::Cos(0.0)));

    // 2. High-Order Time-Series & Financial Indicators
    List<double> listPrices = { 100.0, 102.0, 101.5, 103.0, 104.5, 103.5, 105.0 };
    Console::WriteLine(String::Format("\nAnalyzing sample market price series with {0} bars:", listPrices.GetCount()));

    double dMean = Math::Mean(listPrices);
    double dSampleVar = Math::Variance(listPrices, true);
    double dStdDev = Math::StandardDeviation(listPrices, true);
    Console::WriteLine(String::Format("  Mean Price: {0:F2}", dMean));
    Console::WriteLine(String::Format("  Sample Variance: {0:F4}", dSampleVar));
    Console::WriteLine(String::Format("  Sample Standard Deviation: {0:F4}", dStdDev));

    // 3. Moving Averages (SMA & EMA)
    int iPeriod = 3;
    double dSma = Math::MovingAverage(listPrices, iPeriod);
    double dEma = Math::ExponentialMovingAverage(listPrices, iPeriod);
    Console::WriteLine(String::Format("  Simple Moving Average (Period {0}): {1:F2}", iPeriod, dSma));
    Console::WriteLine(String::Format("  Exponential Moving Average (Period {0}): {1:F2}", iPeriod, dEma));

    // 4. Moving Average Series
    List<double> listSmaSeries = Math::MovingAverageSeries(listPrices, iPeriod);
    List<double> listEmaSeries = Statistics::ExponentialMovingAverageSeries(listPrices, iPeriod);
    Console::WriteLine(String::Format("  Computed {0} SMA rolling windows and {1} EMA points via System::Numerics::Statistics.",
        listSmaSeries.GetCount(), listEmaSeries.GetCount()));
}
