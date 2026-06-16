#include "System/Diagnostics/Stopwatch.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateStopwatch() {
    Console::WriteLine("\n--- Stopwatch Demonstration ---");

    Stopwatch swStopwatch = Stopwatch::StartNew();
    Console::WriteLine("Stopwatch started...");

    Thread::Sleep(150);

    swStopwatch.Stop();
    Console::Write("Elapsed: ");
    Console::Write((double)swStopwatch.Elapsed().GetTotalMilliseconds());
    Console::WriteLine(" ms");
}
