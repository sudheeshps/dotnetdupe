#include "System/TimeProvider.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateTimeProvider() {
    Console::WriteLine("\n--- TimeProvider Demonstration ---");

    std::shared_ptr<TimeProvider> pProvider = TimeProvider::GetSystem();
    DateTimeOffset dtNow = pProvider->GetUtcNow();
    
    Console::Write("UTC Ticks: ");
    Console::WriteLine((long long)dtNow.GetTicks());

    int64_t llStart = pProvider->GetTimestamp();
    Console::WriteLine("Starting operation...");
    Thread::Sleep(50);
    
    TimeSpan tsElapsed = pProvider->GetElapsedTime(llStart);
    Console::Write("Operation took: ");
    Console::Write((double)tsElapsed.GetTotalMilliseconds());
    Console::WriteLine(" ms");
}
