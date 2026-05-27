#include "System/Array.h"
#include "System/String.h"
#include "System/IO/File.h"
#include "System/IO/FileStream.h"
#include "System/Environment.h"
#include "System/Buffer.h"
#include "System/Char.h"
#include "System/Path.h"
#include "System/Random.h"
#include "System/Uri.h"
#include "System/Version.h"
#include "System/OperatingSystem.h"
#include "System/BitConverter.h"
#include "System/Convert.h"
#include "System/TimeProvider.h"
#include "System/Diagnostics/Stopwatch.h"
#include "System/Text/StringBuilder.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Guid.h"
#include "System/Console.h"
#include "System/TimeZone.h"
#include "System/TimeZoneInfo.h"
#include "System/IO/StringReader.h"
#include "System/IO/StringWriter.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Interlocked.h"
#include "System/Threading/Lock.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Text;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::Threading;

void DemonstrateConsole() {
    Console::WriteLine("\n--- Console Demonstration ---");
    Console::Write("MOCK INPUT: Enter your name: ");
    Console::SetIn("Gemini User"); 
    String name = Console::ReadLine();
    Console::Write("Hello, ");
    Console::WriteLine(name);
    
    // Clear mock input so subsequent ReadLine calls wait for actual stdin
    Console::ClearInputs(); 

    Console::WriteLine("Available Colors:");
    Console::SetForegroundColor(ConsoleColor::Green);
    Console::WriteLine("  This is Green text");
    Console::SetForegroundColor(ConsoleColor::Cyan);
    Console::WriteLine("  This is Cyan text");
    Console::ResetColor();
    
    Console::SetTitle("DotNetDupe Demo Application");
}

void DemonstrateString() {
    Console::WriteLine("\n--- String Demonstration ---");
    
    String s1 = "   Hello World   ";
    Console::Write("Original: '");
    Console::Write(s1);
    Console::WriteLine("'");
    
    Console::Write("Trimmed: '");
    Console::Write(s1.Trim());
    Console::WriteLine("'");
    
    Console::Write("Lower: ");
    Console::WriteLine(s1.ToLower());
    
    Console::Write("Upper: ");
    Console::WriteLine(s1.ToUpper());

    String s2 = "Apple,Banana,Cherry";
    auto parts = s2.Split(',');
    Console::WriteLine("Splitting 'Apple,Banana,Cherry':");
    for(const auto& p : parts) {
        Console::Write("  - ");
        Console::WriteLine(p);
    }
}

void DemonstrateBitConverter() {
    Console::WriteLine("\n--- BitConverter Demonstration ---");

    int intVal = 123456789;
    Array<byte> intBytes = BitConverter::GetBytes(intVal);
    Console::Write("Int: 123456789 to bytes: ");
    String hexStr = BitConverter::ToString(intBytes);
    Console::WriteLine(hexStr);

    int backToInt = BitConverter::ToInt32(intBytes, 0);
    Console::Write("Bytes back to int: ");
    Console::WriteLine(backToInt);
}

void DemonstrateConvert() {
    Console::WriteLine("\n--- Convert Demonstration ---");

    // String to numeric
    String intStr("12345");
    int intVal = Convert::ToInt32(intStr);
    Console::Write("String '12345' to Int32: ");
    Console::WriteLine(intVal);

    // Numeric to String
    String boolStr = Convert::ToString(true);
    Console::Write("Bool true to String: ");
    Console::WriteLine(boolStr);

    // Base conversions
    String hexStr = Convert::ToString(255, 16);
    Console::Write("Int 255 to Hex: ");
    Console::WriteLine(hexStr);
}

void DemonstrateTimeProvider() {
    Console::WriteLine("\n--- TimeProvider Demonstration ---");

    auto provider = TimeProvider::GetSystem();
    DateTimeOffset now = provider->GetUtcNow();
    
    Console::Write("UTC Ticks: ");
    Console::WriteLine((long long)now.GetTicks());

    int64_t start = provider->GetTimestamp();
    Console::WriteLine("Starting operation...");
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    TimeSpan elapsed = provider->GetElapsedTime(start);
    Console::Write("Operation took: ");
    Console::Write((double)elapsed.GetTotalMilliseconds());
    Console::WriteLine(" ms");
}

void DemonstrateStopwatch() {
    Console::WriteLine("\n--- Stopwatch Demonstration ---");

    Stopwatch sw = Stopwatch::StartNew();
    Console::WriteLine("Stopwatch started...");

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    sw.Stop();
    Console::Write("Elapsed: ");
    Console::Write((double)sw.Elapsed().GetTotalMilliseconds());
    Console::WriteLine(" ms");
}

void DemonstrateStringBuilder() {
    Console::WriteLine("\n--- StringBuilder Demonstration ---");

    StringBuilder sb;
    sb.Append("User: ")
      .Append("Gemini")
      .AppendLine()
      .Append("Status: ")
      .Append(true)
      .AppendLine()
      .Append("Score: ")
      .Append(98.5);

    Console::Write(sb.ToString());
    Console::WriteLine();
}

void DemonstrateCollections() {
    Console::WriteLine("\n--- Collections Demonstration ---");

    // List
    List<int> numbers = { 5, 2, 8, 1, 9 };
    numbers.Add(4);
    numbers.Sort();
    
    Console::Write("Sorted List: ");
    for (int n : numbers) {
        Console::Write(n);
        Console::Write(" ");
    }
    Console::WriteLine();

    // List Functional
    Console::Write("Numbers > 5: ");
    auto bigNumbers = numbers.FindAll([](int n) { return n > 5; });
    for (int n : bigNumbers) {
        Console::Write(n);
        Console::Write(" ");
    }
    Console::WriteLine();

    // Dictionary
    Dictionary<String, String> meta;
    meta.Add("OS", "Windows");
    meta.Add("Arch", "x64");

    Console::WriteLine("Dictionary Meta:");
    for (auto const& [key, val] : meta) {
        Console::Write("  ");
        Console::Write(key);
        Console::Write(" => ");
        Console::WriteLine(val);
    }
}

void DemonstrateGuid() {
    Console::WriteLine("\n--- Guid Demonstration ---");
    Console::Write("New Guid: ");
    Console::WriteLine(Guid::NewGuid().ToString());
}

void DemonstrateEnvironment() {
    Console::WriteLine("\n--- Environment Demonstration ---");
    Console::Write("OS: ");
    Console::WriteLine(Environment::GetOperatingSystem().GetVersionString());
    Console::Write("Processor Count: ");
    Console::WriteLine((int)Environment::GetProcessorCount());
}

void DemonstrateTimeZone() {
    Console::WriteLine("\n--- TimeZone Demonstration ---");

    TimeZone* current = TimeZone::GetCurrentTimeZone();
    Console::Write("Current TimeZone Standard Name: ");
    Console::WriteLine(current->GetStandardName());
    Console::Write("Current TimeZone Daylight Name: ");
    Console::WriteLine(current->GetDaylightName());

    DateTimeOffset now(638200000000000000LL); // Arbitrary point
    TimeSpan offset = current->GetUtcOffset(now);
    Console::Write("Current UTC Offset (hours): ");
    Console::WriteLine(offset.GetTotalHours());

    TimeZoneInfo local = TimeZoneInfo::Local();
    Console::Write("TimeZoneInfo Local Id: ");
    Console::WriteLine(local.GetId());
    Console::Write("TimeZoneInfo Local Display Name: ");
    Console::WriteLine(local.GetDisplayName());

    TimeZoneInfo utc = TimeZoneInfo::Utc();
    Console::Write("TimeZoneInfo Utc Id: ");
    Console::WriteLine(utc.GetId());
}

void DemonstrateTextIO() {
    Console::WriteLine("\n--- Text I/O Demonstration ---");

    // StringWriter
    StringWriter sw;
    sw.WriteLine("First line");
    sw.WriteLine("Second line");
    sw.Write("Part of third line... ");
    sw.Write("rest of third line.");

    String result = sw.ToString();
    Console::WriteLine("Written content:");
    Console::WriteLine(result);

    // StringReader
    StringReader sr(result);
    Console::WriteLine("Reading back line by line:");
    String line;
    int lineNum = 1;
    while (!(line = sr.ReadLine()).IsEmpty()) {
        Console::Write("Line ");
        Console::Write(lineNum++);
        Console::Write(": ");
        Console::WriteLine(line);
    }
}

void DemonstrateThreading() {
    Console::WriteLine("\n--- Threading Demonstration ---");

    Thread* current = Thread::GetCurrentThread();
    Console::WriteLine("Main Thread active.");

    bool workerExecuted = false;
    Thread worker([&workerExecuted]() {
        Thread* thisThread = Thread::GetCurrentThread();
        thisThread->SetName("WorkerThread");
        Console::Write("Worker Thread Name: ");
        Console::WriteLine(thisThread->GetName());
        
        Thread::Sleep(100);
        workerExecuted = true;
    });

    Console::WriteLine("Starting worker thread...");
    worker.Start();
    
    Console::WriteLine("Waiting for worker thread to finish...");
    worker.Join();

    Console::Write("Worker executed: ");
    Console::WriteLine(Convert::ToString(workerExecuted));
}

void DemonstrateSynchronization() {
    Console::WriteLine("\n--- Thread Synchronization Demonstration ---");

    // 1. ManualResetEvent
    Console::WriteLine("Demonstrating ManualResetEvent...");
    ManualResetEvent mre(false);
    Thread t1([&mre]() {
        Console::WriteLine("  Thread 1 waiting on ManualResetEvent...");
        mre.WaitOne();
        Console::WriteLine("  Thread 1 released from ManualResetEvent.");
    });
    t1.Start();
    Thread::Sleep(100);
    Console::WriteLine("  Main thread signaling ManualResetEvent...");
    mre.Set();
    t1.Join();

    // 1b. Event Timeout
    Console::WriteLine("Demonstrating EventWaitHandle Timeout...");
    try {
        Console::WriteLine("  Waiting 100ms on a non-signaled ManualResetEvent...");
        mre.Reset();
        mre.WaitOne(100);
    }
    catch (const TimeoutException& ex) {
        Console::Write("  Caught expected TimeoutException from Event: ");
        Console::WriteLine(ex.What());
    }

    // 2. Interlocked
    Console::WriteLine("\nDemonstrating Interlocked...");
    Interlocked<int> counter = 0;
    std::vector<Thread*> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(new Thread([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter++; // Atomic increment via operator++
            }
        }));
    }
    for (auto t : threads) t->Start();
    for (auto t : threads) {
        t->Join();
        delete t;
    }
    Console::Write("  Counter value (expected 5000): ");
    Console::WriteLine((int)counter);

    // 3. Exception Handling (SemaphoreFullException and TimeoutException)
    Console::WriteLine("\nDemonstrating Synchronization Exception Handling...");
    Semaphore s(1, 1);
    try {
        Console::WriteLine("  Releasing a full semaphore to trigger exception...");
        s.Release(); // Already at 1, max is 1
    }
    catch (const SemaphoreFullException& ex) {
        Console::Write("  Caught expected exception: ");
        Console::WriteLine(ex.What());
    }

    Mutex m;
    m.WaitOne();
    Thread t3([&m]() {
        try {
            Console::WriteLine("  Thread 3 attempting to acquire locked Mutex with 100ms timeout...");
            m.WaitOne(100);
        }
        catch (const TimeoutException& ex) {
            Console::Write("  Thread 3 caught expected TimeoutException: ");
            Console::WriteLine(ex.What());
        }
    });
    t3.Start();
    t3.Join();
    m.Release();

    // 4. CriticalSection
    Console::WriteLine("\nDemonstrating CriticalSection...");
    CriticalSection cs;
    int sharedResource = 0;
    Thread t2([&cs, &sharedResource]() {
        cs.Enter();
        sharedResource += 10;
        Thread::Sleep(50);
        cs.Leave();
    });
    t2.Start();
    Thread::Sleep(10);
    if (!cs.TryEnter()) {
        Console::WriteLine("  Main thread could not enter CriticalSection (locked by Thread 2).");
    }
    t2.Join();
    Console::Write("  Shared resource value: ");
    Console::WriteLine(sharedResource);

    // 5. Thread Join Timeout
    Console::WriteLine("\nDemonstrating Thread::Join Timeout...");
    Thread t4([]() {
        Thread::Sleep(500); // Sleep longer than timeout
    });
    t4.Start();
    try {
        Console::WriteLine("  Attempting to Join thread with 100ms timeout (should fail)...");
        t4.Join(100);
    }
    catch (const TimeoutException& ex) {
        Console::Write("  Caught expected TimeoutException from Join: ");
        Console::WriteLine(ex.What());
    }
    t4.Join(); // Clean up
}

void DemonstrateLockRAII() {
    Console::WriteLine("\n--- RAII Lock Demonstration ---");

    // 1. Lock<Mutex>
    Console::WriteLine("Demonstrating Lock<Mutex>...");
    Mutex m;
    {
        MutexLock lock(m); // Auto-acquires
        Console::WriteLine("  Mutex acquired via RAII Lock.");
        // ... work ...
    } // Auto-releases
    Console::WriteLine("  Mutex released automatically when scope ended.");

    // 2. Lock<CriticalSection>
    Console::WriteLine("\nDemonstrating Lock<CriticalSection>...");
    CriticalSection cs;
    {
        CriticalSectionLock lock(cs);
        Console::WriteLine("  CriticalSection entered via RAII Lock.");
    }
    Console::WriteLine("  CriticalSection left automatically.");

    // 3. Lock<Semaphore> with release count
    Console::WriteLine("\nDemonstrating Lock<Semaphore> with release count...");
    Semaphore s(2, 2);
    {
        // Acquire 1 unit, specify that destructor should release 1 unit (default behavior)
        SemaphoreLock lock(s); 
        Console::WriteLine("  Semaphore unit acquired.");
    }
    Console::WriteLine("  Semaphore unit released automatically.");
}

int main() {
    DemonstrateConsole();
    DemonstrateString();
    DemonstrateBitConverter();
    DemonstrateConvert();
    DemonstrateTimeProvider();
    DemonstrateStopwatch();
    DemonstrateStringBuilder();
    DemonstrateCollections();
    DemonstrateGuid();
    DemonstrateEnvironment();
    DemonstrateTimeZone();
    DemonstrateTextIO();
    DemonstrateThreading();
    DemonstrateSynchronization();
    DemonstrateLockRAII();
    
    Console::WriteLine("\n--- Demonstration Complete ---");
    Console::WriteLine("Press Enter to exit...");
    Console::ReadLine();
    return 0;
}
