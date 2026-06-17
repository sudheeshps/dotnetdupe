#include "System/Console.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

#include "Demos.h"

void DemonstrateLockRAII() {
    Console::WriteLine("\n--- RAII Lock Demonstration ---");

    // 1. Lock<Mutex>
    Console::WriteLine("Demonstrating Lock<Mutex>...");
    Mutex objMutex;
    {
        MutexLock objLock(objMutex); // Auto-acquires
        Console::WriteLine("  Mutex acquired via RAII Lock.");
        // ... work ...
    } // Auto-releases
    Console::WriteLine("  Mutex released automatically when scope ended.");

    // 2. Lock<CriticalSection>
    Console::WriteLine("\nDemonstrating Lock<CriticalSection>...");
    CriticalSection objCs;
    {
        CriticalSectionLock objLock(objCs);
        Console::WriteLine("  CriticalSection entered via RAII Lock.");
    }
    Console::WriteLine("  CriticalSection left automatically.");

    // 3. Lock<Semaphore> with release count
    Console::WriteLine("\nDemonstrating Lock<Semaphore> with release count...");
    Semaphore objSemaphore(2, 2);
    {
        // Acquire 1 unit, specify that destructor should release 1 unit (default behavior)
        SemaphoreLock objLock(objSemaphore); 
        Console::WriteLine("  Semaphore unit acquired.");
    }
    Console::WriteLine("  Semaphore unit released automatically.");
}
