#include "System/Threading/ThreadPool.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ManualResetEvent.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateThreadPool() {
    Console::WriteLine("\n--- ThreadPool Demonstration ---");
    Console::WriteLine("Queuing task to ThreadPool...");

    ManualResetEvent objMre(false);
    
    ThreadPool::QueueUserWorkItem(WaitCallback([&objMre](Object* pState) {
        Console::WriteLine("  -> Executing inside ThreadPool worker thread!");
        // Simulate work
        Thread::Sleep(500);
        Console::WriteLine("  -> ThreadPool work complete.");
        objMre.Set();
    }));

    Console::WriteLine("Main thread waiting for ThreadPool task to finish...");
    objMre.WaitOne();
}
