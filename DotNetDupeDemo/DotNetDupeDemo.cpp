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
#include "System/Diagnostics/Process.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Text/StringBuilder.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/UdpClient.h"
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
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Text;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Net::Sockets;

void DemonstrateConsole() {
    Console::WriteLine("\n--- Console Demonstration ---");
    Console::Write("MOCK INPUT: Enter your name: ");
    Console::SetIn("Gemini User"); 
    String sName = Console::ReadLine();
    Console::Write("Hello, ");
    Console::WriteLine(sName);
    
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
    
    String sS1 = "   Hello World   ";
    Console::Write("Original: '");
    Console::Write(sS1);
    Console::WriteLine("'");
    
    Console::Write("Trimmed: '");
    Console::Write(sS1.Trim());
    Console::WriteLine("'");
    
    Console::Write("Lower: ");
    Console::WriteLine(sS1.ToLower());
    
    Console::Write("Upper: ");
    Console::WriteLine(sS1.ToUpper());

    String sS2 = "Apple,Banana,Cherry";
    Array<String> arrParts = sS2.Split(',');
    Console::WriteLine("Splitting 'Apple,Banana,Cherry':");
    for(const auto& sPart : arrParts) {
        Console::Write("  - ");
        Console::WriteLine(sPart);
    }
}

void DemonstrateBitConverter() {
    Console::WriteLine("\n--- BitConverter Demonstration ---");

    int iIntVal = 123456789;
    Array<byte> arrIntBytes = BitConverter::GetBytes(iIntVal);
    Console::Write("Int: 123456789 to bytes: ");
    String sHexStr = BitConverter::ToString(arrIntBytes);
    Console::WriteLine(sHexStr);

    int iBackToInt = BitConverter::ToInt32(arrIntBytes, 0);
    Console::Write("Bytes back to int: ");
    Console::WriteLine(iBackToInt);
}

void DemonstrateConvert() {
    Console::WriteLine("\n--- Convert Demonstration ---");

    // String to numeric
    String sIntStr("12345");
    int iIntVal = Convert::ToInt32(sIntStr);
    Console::Write("String '12345' to Int32: ");
    Console::WriteLine(iIntVal);

    // Numeric to String
    String sBoolStr = Convert::ToString(true);
    Console::Write("Bool true to String: ");
    Console::WriteLine(sBoolStr);

    // Base conversions
    String sHexStr = Convert::ToString(255, 16);
    Console::Write("Int 255 to Hex: ");
    Console::WriteLine(sHexStr);
}

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

void DemonstrateStringBuilder() {
    Console::WriteLine("\n--- StringBuilder Demonstration ---");

    StringBuilder sbBuilder;
    sbBuilder.Append("User: ")
      .Append("Gemini")
      .AppendLine()
      .Append("Status: ")
      .Append(true)
      .AppendLine()
      .Append("Score: ")
      .Append(98.5);

    Console::Write(sbBuilder.ToString());
    Console::WriteLine();
}

void DemonstrateCollections() {
    Console::WriteLine("\n--- Collections Demonstration ---");

    // List
    List<int> lstNumbers = { 5, 2, 8, 1, 9 };
    lstNumbers.Add(4);
    lstNumbers.Sort();
    
    Console::Write("Sorted List: ");
    for (int iN : lstNumbers) {
        Console::Write(iN);
        Console::Write(" ");
    }
    Console::WriteLine();

    // List Functional
    Console::Write("Numbers > 5: ");
    List<int> lstBigNumbers = lstNumbers.FindAll([](int iN) { return iN > 5; });
    for (int iN : lstBigNumbers) {
        Console::Write(iN);
        Console::Write(" ");
    }
    Console::WriteLine();

    // Dictionary
    Dictionary<String, String> dictMeta;
    dictMeta.Add("OS", "Windows");
    dictMeta.Add("Arch", "x64");

    Console::WriteLine("Dictionary Meta:");
    for (auto const& [sKey, sVal] : dictMeta) {
        Console::Write("  ");
        Console::Write(sKey);
        Console::Write(" => ");
        Console::WriteLine(sVal);
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

    TimeZone* pCurrent = TimeZone::GetCurrentTimeZone();
    Console::Write("Current TimeZone Standard Name: ");
    Console::WriteLine(pCurrent->GetStandardName());
    Console::Write("Current TimeZone Daylight Name: ");
    Console::WriteLine(pCurrent->GetDaylightName());

    DateTimeOffset dtNow(638200000000000000LL); // Arbitrary point
    TimeSpan tsOffset = pCurrent->GetUtcOffset(dtNow);
    Console::Write("Current UTC Offset (hours): ");
    Console::WriteLine(tsOffset.GetTotalHours());

    TimeZoneInfo objLocal = TimeZoneInfo::Local();
    Console::Write("TimeZoneInfo Local Id: ");
    Console::WriteLine(objLocal.GetId());
    Console::Write("TimeZoneInfo Local Display Name: ");
    Console::WriteLine(objLocal.GetDisplayName());

    TimeZoneInfo objUtc = TimeZoneInfo::Utc();
    Console::Write("TimeZoneInfo Utc Id: ");
    Console::WriteLine(objUtc.GetId());
}

void DemonstrateTextIO() {
    Console::WriteLine("\n--- Text I/O Demonstration ---");

    // StringWriter
    StringWriter pWriter;
    pWriter.WriteLine("First line");
    pWriter.WriteLine("Second line");
    pWriter.Write("Part of third line... ");
    pWriter.Write("rest of third line.");

    String sResult = pWriter.ToString();
    Console::WriteLine("Written content:");
    Console::WriteLine(sResult);

    // StringReader
    StringReader pReader(sResult);
    Console::WriteLine("Reading back line by line:");
    String sLine;
    int iLineNum = 1;
    while (!(sLine = pReader.ReadLine()).IsEmpty()) {
        Console::Write("Line ");
        Console::Write(iLineNum++);
        Console::Write(": ");
        Console::WriteLine(sLine);
    }
}

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
}

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

void DemonstrateFileAttributes() {
    Console::WriteLine("\n--- File Attributes Demonstration ---");

    String sPath = "attr_demo.txt";
    File::WriteAllText(sPath, "Demo content for attributes");

    FileAttributes objAttrs;
    if (File::GetAttributes(sPath, objAttrs)) {
        Console::Write("Initial Attributes (int): ");
        Console::WriteLine((int)objAttrs);

        // Set to ReadOnly
        if (File::SetAttributes(sPath, FileAttributes::ReadOnly)) {
            Console::WriteLine("Set attribute to ReadOnly: Success");
            
            FileAttributes objNewAttrs;
            File::GetAttributes(sPath, objNewAttrs);
            bool bIsReadOnly = ((int)objNewAttrs & (int)FileAttributes::ReadOnly) != 0;
            Console::Write("Is ReadOnly now? ");
            Console::WriteLine(Convert::ToString(bIsReadOnly));
        }

        // Restore to Normal
        File::SetAttributes(sPath, FileAttributes::Normal);
        Console::WriteLine("Restored attributes to Normal.");
    }

    File::Delete(sPath);
}

void DemonstrateProcess() {
    Console::WriteLine("\n--- Process Demonstration ---");

#if defined(_WIN32)
    String sFileName = "cmd.exe";
    String sArguments = "/c echo Hello from child process! && exit 42";
#else
    String sFileName = "/bin/sh";
    String sArguments = "-c \"echo Hello from child process!; exit 42\"";
#endif

    Console::Write("Starting process: ");
    Console::WriteLine(sFileName);

    auto pProcess = Process::Start(sFileName, sArguments);
    if (!pProcess.IsNull()) {
        Console::Write("Process started with ID: ");
        Console::WriteLine(pProcess->GetId());

        Console::WriteLine("Waiting for process to exit...");
        pProcess->WaitForExit();

        Console::Write("Process exited with code: ");
        Console::WriteLine(pProcess->GetExitCode());
    } else {
        Console::WriteLine("Failed to start process.");
    }
}

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

struct DemoPerson {
    String Name;
    int Age;
};

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<::DemoPerson> {
                    static JsonElement Write(const ::DemoPerson& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("Name", JsonConverter<String>::Write(value.Name));
                        obj.SetProperty("Age", JsonConverter<int>::Write(value.Age));
                        return obj;
                    }

                    static ::DemoPerson Read(const JsonElement& element) {
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw std::runtime_error("Expected a JSON object");
                        }
                        ::DemoPerson p;
                        JsonElement prop;
                        if (element.TryGetProperty("Name", prop)) {
                            p.Name = JsonConverter<String>::Read(prop);
                        }
                        if (element.TryGetProperty("Age", prop)) {
                            p.Age = JsonConverter<int>::Read(prop);
                        }
                        return p;
                    }
                };
            }
        }
    }
}

void DemonstrateJson() {
    using namespace DotNetDupe::System::Text::Json;
    Console::WriteLine("\n--- JSON Serialization/Deserialization Demonstration ---");

    // 1. Primitives
    int iOriginal = 123;
    String sJsonInt = JsonSerializer::Serialize(iOriginal);
    Console::Write("Serialized int: ");
    Console::WriteLine(sJsonInt);

    int iDeserialized = JsonSerializer::Deserialize<int>(sJsonInt);
    Console::Write("Deserialized int: ");
    Console::WriteLine(Convert::ToString(iDeserialized));

    // 2. List
    List<int> lstNumbers = { 10, 20, 30, 40 };
    String sJsonList = JsonSerializer::Serialize(lstNumbers);
    Console::Write("Serialized List<int>: ");
    Console::WriteLine(sJsonList);

    List<int> lstDeserialized = JsonSerializer::Deserialize<List<int>>(sJsonList);
    Console::Write("Deserialized List<int> (count): ");
    Console::WriteLine(Convert::ToString(lstDeserialized.GetCount()));

    // 3. Custom Object
    DemoPerson objPerson = { "Bob", 25 };
    String sJsonObj = JsonSerializer::Serialize(objPerson);
    Console::Write("Serialized DemoPerson: ");
    Console::WriteLine(sJsonObj);

    DemoPerson objDeserialized = JsonSerializer::Deserialize<DemoPerson>(sJsonObj);
    Console::Write("Deserialized DemoPerson - Name: '");
    Console::Write(objDeserialized.Name);
    Console::Write("', Age: ");
    Console::WriteLine(Convert::ToString(objDeserialized.Age));
}

void DemonstrateSockets() {
    Console::WriteLine("\n--- Sockets Demonstration ---");
    int port = 19090;

    Console::WriteLine("Starting TcpListener on 127.0.0.1:19090...");
    TcpListener listener("127.0.0.1", port);
    listener.Start();

    Thread serverThread([&listener]() {
        try {
            auto serverClient = listener.AcceptTcpClient();
            Console::WriteLine("  [Server] Accepted client connection!");
            auto stream = serverClient->GetStream();

            char buffer[128] = {0};
            int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                Console::Write("  [Server] Received: '");
                Console::Write(buffer);
                Console::WriteLine("'");
            }

            String response = "World";
            stream->Write(response.GetRawString(), 0, response.GetLength());
            serverClient->Close();
        } catch (const DotNetDupe::System::SystemException& ex) {
            Console::Write("  [Server] SystemException: ");
            Console::WriteLine(ex.What());
        } catch (const std::exception& ex) {
            Console::Write("  [Server] std::exception: ");
            Console::WriteLine(ex.what());
        }
    });

    serverThread.Start();
    Thread::Sleep(100);

    try {
        Console::WriteLine("Connecting TcpClient to 127.0.0.1:19090...");
        TcpClient client;
        client.Connect("127.0.0.1", port);

        auto stream = client.GetStream();
        String msg = "Hello";
        stream->Write(msg.GetRawString(), 0, msg.GetLength());

        char buffer[128] = {0};
        int bytesRead = stream->Read(buffer, 0, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            Console::Write("  [Client] Received back: '");
            Console::Write(buffer);
            Console::WriteLine("'");
        }
        client.Close();
    } catch (const DotNetDupe::System::SystemException& ex) {
        Console::Write("  [Client] SystemException: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("  [Client] std::exception: ");
        Console::WriteLine(ex.what());
    }

    serverThread.Join();
    listener.Stop();
    Console::WriteLine("TcpListener stopped.");
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
    DemonstrateFileAttributes();
    DemonstrateThreading();
    DemonstrateSynchronization();
    DemonstrateLockRAII();
    DemonstrateProcess();
    DemonstrateThreadPool();
    DemonstrateTask();
    DemonstrateJson();
    DemonstrateSockets();
    
    Console::WriteLine("\n--- Demonstration Complete ---");
    Console::WriteLine("Press Enter to exit...");
    Console::ReadLine();
    return 0;
}
