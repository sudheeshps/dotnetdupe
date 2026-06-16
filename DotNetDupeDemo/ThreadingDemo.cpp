#include "System/Convert.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateThreading() {
    Console::WriteLine("\n--- Threading Demonstration ---");

    Thread* pCurrentThread = Thread::GetCurrentThread();
    Console::WriteLine("Main Thread active.");

    bool bWorkerExecuted = false;
    Thread pThreadWorker([&bWorkerExecuted]() {
        Thread* pThisThread = Thread::GetCurrentThread();
        pThisThread->SetName("WorkerThread");
        Console::Write("Worker Thread Name: ");
        Console::WriteLine(pThisThread->GetName());
        
        Thread::Sleep(100);
        bWorkerExecuted = true;
    });

    Console::WriteLine("Starting worker thread...");
    pThreadWorker.Start();
    
    Console::WriteLine("Waiting for worker thread to finish...");
    pThreadWorker.Join();

    Console::Write("Worker executed: ");
    Console::WriteLine(Convert::ToString(bWorkerExecuted));
}
