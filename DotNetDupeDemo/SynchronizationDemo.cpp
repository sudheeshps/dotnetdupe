#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Interlocked.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateSynchronization() {
    Console::WriteLine("\n--- Thread Synchronization Demonstration ---");

    // 1. ManualResetEvent
    Console::WriteLine("Demonstrating ManualResetEvent...");
    ManualResetEvent objMre(false);
    Thread pThread1([&objMre]() {
        Console::WriteLine("  Thread 1 waiting on ManualResetEvent...");
        objMre.WaitOne();
        Console::WriteLine("  Thread 1 released from ManualResetEvent.");
    });
    pThread1.Start();
    Thread::Sleep(100);
    Console::WriteLine("  Main thread signaling ManualResetEvent...");
    objMre.Set();
    pThread1.Join();

    // 1b. Event Timeout
    Console::WriteLine("Demonstrating EventWaitHandle Timeout...");
    try {
        Console::WriteLine("  Waiting 100ms on a non-signaled ManualResetEvent...");
        objMre.Reset();
        objMre.WaitOne(100);
    }
    catch (const TimeoutException& ex) {
        Console::Write("  Caught expected TimeoutException from Event: ");
        Console::WriteLine(ex.What());
    }

    // 2. Interlocked
    Console::WriteLine("\nDemonstrating Interlocked...");
    Interlocked<int> objCounter = 0;
    std::vector<Thread*> lstThreads;
    for (int iI = 0; iI < 5; ++iI) {
        lstThreads.push_back(new Thread([&objCounter]() {
            for (int iJ = 0; iJ < 1000; ++iJ) {
                objCounter++; // Atomic increment via operator++
            }
        }));
    }
    for (auto pT : lstThreads) pT->Start();
    for (auto pT : lstThreads) {
        pT->Join();
        delete pT;
    }
    Console::Write("  Counter value (expected 5000): ");
    Console::WriteLine((int)objCounter);

    // 3. Exception Handling (SemaphoreFullException and TimeoutException)
    Console::WriteLine("\nDemonstrating Synchronization Exception Handling...");
    Semaphore objSemaphore(1, 1);
    try {
        Console::WriteLine("  Releasing a full semaphore to trigger exception...");
        objSemaphore.Release(); // Already at 1, max is 1
    }
    catch (const SemaphoreFullException& ex) {
        Console::Write("  Caught expected exception: ");
        Console::WriteLine(ex.What());
    }

    Mutex objMutex;
    objMutex.WaitOne();
    Thread pThread3([&objMutex]() {
        try {
            Console::WriteLine("  Thread 3 attempting to acquire locked Mutex with 100ms timeout...");
            objMutex.WaitOne(100);
        }
        catch (const TimeoutException& ex) {
            Console::Write("  Thread 3 caught expected TimeoutException: ");
            Console::WriteLine(ex.What());
        }
    });
    pThread3.Start();
    pThread3.Join();
    objMutex.Release();

    // 4. CriticalSection
    Console::WriteLine("\nDemonstrating CriticalSection...");
    CriticalSection objCs;
    int iSharedResource = 0;
    Thread pThread2([&objCs, &iSharedResource]() {
        objCs.Enter();
        iSharedResource += 10;
        Thread::Sleep(50);
        objCs.Leave();
    });
    pThread2.Start();
    Thread::Sleep(10);
    if (!objCs.TryEnter()) {
        Console::WriteLine("  Main thread could not enter CriticalSection (locked by Thread 2).");
    }
    pThread2.Join();
    Console::Write("  Shared resource value: ");
    Console::WriteLine(iSharedResource);

    // 5. Thread Join Timeout
    Console::WriteLine("\nDemonstrating Thread::Join Timeout...");
    Thread pThread4([]() {
        Thread::Sleep(500); // Sleep longer than timeout
    });
    pThread4.Start();
    try {
        Console::WriteLine("  Attempting to Join thread with 100ms timeout (should fail)...");
        pThread4.Join(100);
    }
    catch (const TimeoutException& ex) {
        Console::Write("  Caught expected TimeoutException from Join: ");
        Console::WriteLine(ex.What());
    }
    pThread4.Join(); // Clean up

    // 6. Named Synchronization Objects
    Console::WriteLine("\nDemonstrating Named Synchronization Objects...");
    bool bCreatedNewEvent = false;
    ManualResetEvent objNamedEvent(false, "GlobalDemoEvent", true, bCreatedNewEvent);
    Console::Write("  Created named ManualResetEvent (bCreatedNew=");
    Console::Write(bCreatedNewEvent ? "true" : "false");
    Console::WriteLine(")");

    ManualResetEvent* pOpenedEvent = ManualResetEvent::OpenExisting("GlobalDemoEvent");
    if (pOpenedEvent != nullptr) {
        Console::WriteLine("  Successfully opened existing named ManualResetEvent via OpenExisting.");
    }

    bool bCreatedNewMutex = false;
    Mutex objNamedMutex(false, "GlobalDemoMutex", true, bCreatedNewMutex);
    Console::Write("  Created named Mutex (bCreatedNew=");
    Console::Write(bCreatedNewMutex ? "true" : "false");
    Console::WriteLine(")");

    bool bCreatedNewSem = false;
    Semaphore objNamedSem(1, 1, "GlobalDemoSemaphore", true, bCreatedNewSem);
    Console::Write("  Created named Semaphore (bCreatedNew=");
    Console::Write(bCreatedNewSem ? "true" : "false");
    Console::WriteLine(")");
}
