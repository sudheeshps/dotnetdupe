#include "System/Convert.h"
#include "System/Threading/Tasks/Task.h"

#include "System/Console.h"
#include "System/Threading/Thread.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateTask() {
    Console::WriteLine("\n--- Task Demonstration ---");
    Console::WriteLine("Starting asynchronous Task...");

    auto pTask = DotNetDupe::System::Threading::Tasks::Task::Run(Action<>([]() {
        Console::WriteLine("  -> Task is running...");
        Thread::Sleep(500);
        Console::WriteLine("  -> Task complete.");
    }));

    Console::WriteLine("Main thread waiting on Task::Wait()...");
    pTask->Wait();
    
    Console::Write("Task completed successfully? ");
    Console::WriteLine(Convert::ToString(pTask->GetIsCompleted() && !pTask->GetIsFaulted()));
}
