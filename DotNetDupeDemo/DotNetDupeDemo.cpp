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
#include "System/InvalidOperationException.h"
#include "System/Net/Sockets/Socket.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Sockets/UdpClient.h"
#include "System/Net/Dns.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Net/Security/SslStream.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/IServiceProvider.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
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
                            throw InvalidOperationException("Expected a JSON object");
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

    try {
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
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const DotNetDupe::System::BasicException<char>& ex) {
                Console::Write("  [Server] BasicException: ");
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
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::BasicException<char>& ex) {
            Console::Write("  [Client] BasicException: ");
            Console::WriteLine(ex.What());
        } catch (const std::exception& ex) {
            Console::Write("  [Client] std::exception: ");
            Console::WriteLine(ex.what());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("TcpListener stopped.");
    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
        Console::Write("  [Demo] SocketException: ");
        Console::WriteLine(ex.What());
    } catch (const DotNetDupe::System::BasicException<char>& ex) {
        Console::Write("  [Demo] BasicException: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("  [Demo] std::exception: ");
        Console::WriteLine(ex.what());
    }
}

void DemonstrateHttp() {
    Console::WriteLine("\n=== Demonstrate HTTP Client ===");
    int port = 19091;

    try {
        Console::WriteLine("Starting mock HTTP server on 127.0.0.1:19091...");
        TcpListener listener("127.0.0.1", port);
        listener.Start();

        Thread serverThread([&listener]() {
            try {
                auto serverClient = listener.AcceptTcpClient();
                Console::WriteLine("  [Server] Accepted client HTTP connection!");
                auto stream = serverClient->GetStream();

                // Simple request reader (read until headers end)
                char c;
                std::string reqLine;
                while (stream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (reqLine == "\r" || reqLine.empty()) break;
                        reqLine.clear();
                    } else {
                        reqLine += c;
                    }
                }

                // Send back a mock HTTP response
                std::string response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain; charset=utf-8\r\n"
                    "Content-Length: 34\r\n"
                    "Server: DotNetDupeMockServer/1.0\r\n"
                    "\r\n"
                    "Hello from DotNetDupe HTTP Server!";
                stream->Write(response.c_str(), 0, static_cast<int>(response.length()));
                serverClient->Close();
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const BasicException<char>& ex) {
                Console::Write("  [Server] BasicException: ");
                Console::WriteLine(ex.What());
            } catch (const std::exception& ex) {
                Console::Write("  [Server] Exception: ");
                Console::WriteLine(ex.what());
            }
        });

        serverThread.Start();
        Thread::Sleep(100);

        try {
            using namespace DotNetDupe::System::Net::Http;
            HttpClient client;
            client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupeClient/1.0");

            Console::WriteLine("Connecting HttpClient to http://127.0.0.1:19091/...");
            auto response = client.Get("http://127.0.0.1:19091/");

            response->EnsureSuccessStatusCode();

            Console::Write("Response Status Code: ");
            Console::WriteLine(static_cast<int>(response->GetStatusCode()));

            Console::Write("Server header: ");
            Console::WriteLine(response->GetHeaders()["Server"]);

            auto content = response->GetContent();
            if (!content.IsNull()) {
                Console::Write("Response body: '");
                Console::Write(content->ReadAsString());
                Console::WriteLine("'");
            }
        } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
            Console::Write("  [Client] HTTP RequestException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] HTTP SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const BasicException<char>& ex) {
            Console::Write("  [Client] HTTP Exception: ");
            Console::WriteLine(ex.What());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("Mock HTTP server stopped.");
    } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
        Console::Write("  [Demo] HTTP RequestException: ");
        Console::WriteLine(ex.What());
    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
        Console::Write("  [Demo] HTTP SocketException: ");
        Console::WriteLine(ex.What());
    } catch (const BasicException<char>& ex) {
        Console::Write("  [Demo] HTTP Exception: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("  [Demo] std::exception: ");
        Console::WriteLine(ex.what());
    }
}

void DemonstrateHttps() {
    using namespace DotNetDupe::System::Net::Security;
    using namespace DotNetDupe::System::Security::Cryptography::X509Certificates;
    Console::WriteLine("\n=== Demonstrate HTTPS Client & Server ===");
    int iPort = 19092;
    String sCertPath = "demo_test_cert.pem";
    String sKeyPath = "demo_test_key.pem";

    // Write temp certificates for the demo
    IO::File::WriteAllText(sCertPath, 
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDCTCCAfGgAwIBAgIUOw6LfZ3oCoBlz1U0uvdKQArrtkkwDQYJKoZIhvcNAQEL\n"
        "BQAwFDESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTI2MDYxNjA3NTAzN1oXDTM2MDYx\n"
        "MzA3NTAzN1owFDESMBAGA1UEAwwJbG9jYWxob3N0MIIBIjANBgkqhkiG9w0BAQEF\n"
        "AAOCAQ8AMIIBCgKCAQEA3uZU9MHVG1hQB98STU7HfWOvsvnNVdQShmWp8uxpYua3\n"
        "dQ/6hYfTCISDK82QQ4trv9qBN33QzT0PbfQVg/+EZe+mCZdSOlxR61AKpApTY8GG\n"
        "n+T4Vc+WJKUPlEkYY8eP8hP0SQNjH+oPhjvEyHhYL0+Bgv2bhi8QhrSzkG/2C28u\n"
        "jmCWz19nj0+x2HJRcQzQ2NyBGfyY1SecSs+aZ49hpAgGL4hdgqP7klQNnIjyICt9\n"
        "ldMYIyMxEOIyZFByVjCda2Kyet+UFZUxDFxOnqpUOLpWNhnGd4L9S94xS/UE1PC0\n"
        "v/QYa2AwR3+4BCLZLuXllPTgdaSWRn5TFDkAojE2dQIDAQABo1MwUTAdBgNVHQ4E\n"
        "FgQUIJd2NudarHCx1F6ev4BNf/KTKM4wHwYDVR0jBBgwFoAUIJd2NudarHCx1F6e\n"
        "v4BNf/KTKM4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAdDVb\n"
        "AgxQIyknjuLEjhyjYlOBfvxmfneuMULa2EZcx6C+gZW+WvrKttMl972RP6Bj71Rc\n"
        "KaXUpf3l2MuRqNIVNj7gWIPyQDN7bJpY0zxTneRMNgYwLn+YxB5b3AZb2/1XsfvJ\n"
        "ynncvVbLkSKHxMJHj/BMUbNkea4Z9RuU5aoBsBCO/GCYfQsf0g5aWcXCpepV6/Dq\n"
        "ur+G8JIUFCmjihPipYjDLPw4IHtuKQMKr43rZHMoAeyfphbX6RlS0NH0X0LOGTOL\n"
        "CGariC58/NHl8dbdQz3JZ/EI5D+ZK7QgnEBoFrot6lLfEJHZEYrmn2WuyBmxHZNC\n"
        "p5nX46yXDcMYhJlhEg==\n"
        "-----END CERTIFICATE-----\n");

    IO::File::WriteAllText(sKeyPath, 
        "-----BEGIN PRIVATE KEY-----\n"
        "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDe5lT0wdUbWFAH\n"
        "3xJNTsd9Y6+y+c1V1BKGZany7Gli5rd1D/qFh9MIhIMrzZBDi2u/2oE3fdDNPQ9t\n"
        "9BWD/4Rl76YJl1I6XFHrUAqkClNjwYaf5PhVz5YkpQ+USRhjx4/yE/RJA2Mf6g+G\n"
        "O8TIeFgvT4GC/ZuGLxCGtLOQb/YLby6OYJbPX2ePT7HYclFxDNDY3IEZ/JjVJ5xK\n"
        "z5pnj2GkCAYviF2Co/uSVA2ciPIgK32V0xgjIzEQ4jJkUHJWMJ1rYrJ635QVlTEM\n"
        "XE6eqlQ4ulY2GcZ3gv1L3jFL9QTU8LS/9BhrYDBHf7gEItku5eWU9OB1pJZGflMU\n"
        "OQCiMTZ1AgMBAAECggEAA1cEMt6sqMo7+MI47fueNZzGxzHDB+QzmtvvHfq44Yh/\n"
        "ezuQD2wnl1sU9BUSclCmfqtxgDlcZZbiok0j/WRYwYZDwKUJ0GINwC7k2exlr3r4\n"
        "33B44CBKKaZng1tNCEG3QHNYF7f7RkEllmdS3YYgHCVe1W2jZPFTmBEhF6r1ztoG\n"
        "eSAw6FaEEPzCUUNiMP2Cc3/MUEjdbs+kYPIoTab7HoKGoWTz4GN+DlCDB8hxwjZw\n"
        "PRScWmlH79ji3IqH7zbKZf7vXIUxz2JyMh9Bplbs0FZU3NFXUFBA0wn3Zk0PZsL0\n"
        "hPMBo7G1WoMskilJydhFI2MJyMKgsI2Q8IeXRqZmQQKBgQD2CXqEmf7txv3ELTM3\n"
        "weavoXpFkb/Uc6PjDMKNkxG5stqmtZAkFrr25966khiYS3GZvOSsR0IvZ8ZmZIiU\n"
        "iebewmUHeiTcZxUHL2pxc7zXpV3cO2o0+EFnRQZUdYa6imursw5YmSjCdSSpGV2k\n"
        "HFN9qy0mpHrcO8OkJPxpeCe2SQKBgQDn7QCnL8q7m0QIQPY/pdeL1ldVDvgEmIBI\n"
        "/cfm0YyTOC2Y2X3jkJJaOSt1YBU6YGfqOrrzrIc8zemPJXqT0+t5od2CehEuaAwB\n"
        "DAMhDZSM7k4WGFoG5hT4wMV/9h2NmKJrnfdSq0hcjvWgjoWciGO/EuxwWrOECA48\n"
        "uerIogpOzQKBgB2neM/gq7xG/rEVbZXr1f0XLmCSN5fItFkaYZykvD8wknADdDMV\n"
        "VkWKHutHzpUUisWn4bvt/skt7djLhza4BE6XCYxEKYszFXK/QUoZVAp90PlJY/ys\n"
        "a12iESg+3hU/t8iDFViSZTpC2j9TYlj0XQ/JgPCmZHSE84imOJIRvF+ZAoGAChmS\n"
        "G7XDGGRslAdwMdlhB5/v8a5TMrAJdiRHHlAl1QtvQObHyq8zEGbI87WZdw9XtyWT\n"
        "mqT6xrUY5Lj3G5Tw/j0ETnhOYzz4uNrzD+9tdq2yDNhGo2JFq4eJkkb/hnkNKa6Z\n"
        "ZKo83nsDS1ipjOWLoniKf2ITyz4mkJJwHQihdkkCgYEA07WQ8VbbHzqkXBdTizMb\n"
        "cmg1Fcr7Pa+oLdVq6kR1scG2zO07Bdn7QazJJONoWoBgdXYuTAJ4BFwDktJ75iLW\n"
        "gReIy0WeSxPdEUIzHnPIORC6ksYPoV/J2unCEUX1OrATd2ubAmTVgcIREqbyh3IB\n"
        "vy4bNlvZuoWwBMUqyVyUNlw=\n"
        "-----END PRIVATE KEY-----\n");

    try {
        auto spCert = SmartPointer<X509Certificate2>::NewShared(sCertPath, sKeyPath);

        Console::WriteLine("Starting mock HTTPS server on 127.0.0.1:19092...");
        TcpListener listener("127.0.0.1", iPort);
        listener.Start();

        Thread serverThread([&listener, spCert]() {
            try {
                auto spServerClient = listener.AcceptTcpClient();
                Console::WriteLine("  [Server] Accepted client HTTPS connection!");
                auto spInnerStream = spServerClient->GetStream();
                
                auto spSslStream = SmartPointer<Net::Security::SslStream>::NewShared(spInnerStream, false);
                spSslStream->AuthenticateAsServer(spCert);

                // Simple request reader (read until headers end)
                char c;
                std::string sReqLine;
                while (spSslStream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (sReqLine == "\r" || sReqLine.empty()) break;
                        sReqLine.clear();
                    } else {
                        sReqLine += c;
                    }
                }

                // Send back a mock HTTPS response
                std::string sResponse = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain; charset=utf-8\r\n"
                    "Content-Length: 35\r\n"
                    "Server: DotNetDupeMockHttpsServer/1.0\r\n"
                    "\r\n"
                    "Hello from DotNetDupe HTTPS Server!";
                spSslStream->Write(sResponse.c_str(), 0, static_cast<int>(sResponse.length()));
                spServerClient->Close();
            } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                Console::Write("  [Server] SocketException: ");
                Console::WriteLine(ex.What());
            } catch (const BasicException<char>& ex) {
                Console::Write("  [Server] BasicException: ");
                Console::WriteLine(ex.What());
            } catch (const std::exception& ex) {
                Console::Write("  [Server] Exception: ");
                Console::WriteLine(ex.what());
            }
        });

        serverThread.Start();
        Thread::Sleep(100);

        try {
            using namespace DotNetDupe::System::Net::Http;
            HttpClient client;
            client.GetDefaultRequestHeaders().Add("User-Agent", "DotNetDupeClient/1.0");

            Console::WriteLine("Connecting HttpClient to https://127.0.0.1:19092/...");
            auto spResponse = client.Get("https://127.0.0.1:19092/");

            spResponse->EnsureSuccessStatusCode();

            Console::Write("Response Status Code: ");
            Console::WriteLine(static_cast<int>(spResponse->GetStatusCode()));

            Console::Write("Server header: ");
            Console::WriteLine(spResponse->GetHeaders()["Server"]);

            auto spContent = spResponse->GetContent();
            if (!spContent.IsNull()) {
                Console::Write("Response body: '");
                Console::Write(spContent->ReadAsString());
                Console::WriteLine("'");
            }
        } catch (const DotNetDupe::System::Net::Http::HttpRequestException& ex) {
            Console::Write("  [Client] HTTPS RequestException: ");
            Console::WriteLine(ex.What());
        } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
            Console::Write("  [Client] HTTPS SocketException: ");
            Console::WriteLine(ex.What());
        } catch (const BasicException<char>& ex) {
            Console::Write("  [Client] HTTPS Exception: ");
            Console::WriteLine(ex.What());
        }

        serverThread.Join();
        listener.Stop();
        Console::WriteLine("Mock HTTPS server stopped.");
    } catch (const BasicException<char>& ex) {
        Console::Write("  [Demo] HTTPS Exception: ");
        Console::WriteLine(ex.What());
    }

    // Clean up cert files
    IO::File::Delete(sCertPath);
    IO::File::Delete(sKeyPath);
}

void DemonstrateJwt() {
    Console::WriteLine("\n=== Demonstrate JWT Token ===");
    try {
        using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;

        // 1. Create token
        JWTToken token;
        token.GetPayload().Add("iss", "dot-net-dupe-auth");
        token.GetPayload().Add("sub", "user_98765");
        token.GetPayload().Add("scope", "read write");

        String secret = "secure-signature-secret-key";

        Console::WriteLine("Generating signed JWT token...");
        String tokenStr = token.CreateToken(secret);
        Console::Write("JWT Token string: ");
        Console::WriteLine(tokenStr);

        // 2. Parse token
        Console::WriteLine("Parsing token...");
        auto parsedToken = JWTToken::Parse(tokenStr);
        
        Console::Write("  Subject claim: ");
        Console::WriteLine(parsedToken->GetPayload()["sub"]);
        Console::Write("  Scope claim:   ");
        Console::WriteLine(parsedToken->GetPayload()["scope"]);

        // 3. Verify signature
        Console::Write("Verifying signature with correct key:   ");
        Console::WriteLine(parsedToken->Verify(secret) ? "VALID" : "INVALID");

        Console::Write("Verifying signature with incorrect key: ");
        Console::WriteLine(parsedToken->Verify("some-other-secret-key") ? "VALID" : "INVALID");

    } catch (const BasicException<char>& ex) {
        Console::Write("Error during JWT demonstration: ");
        Console::WriteLine(ex.What());
    }
}

namespace DependencyInjectionDemo {
    class IDatabaseConnection : public virtual DotNetDupe::System::Object, public DotNetDupe::System::IO::IDisposable {
    public:
        virtual ~IDatabaseConnection() = default;
        virtual void Query(const char* query) = 0;
    };

    class SqlDatabaseConnection : public IDatabaseConnection {
    public:
        SqlDatabaseConnection() {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Constructed.");
        }
        ~SqlDatabaseConnection() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Destructed.");
        }
        void Query(const char* query) override {
            DotNetDupe::System::Console::Write("  [SqlDatabaseConnection] Executing query: ");
            DotNetDupe::System::Console::WriteLine(query);
        }
        void Dispose() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Disposed.");
        }
    };

    class IRepository : public virtual DotNetDupe::System::Object {
    public:
        virtual ~IRepository() = default;
        virtual void SaveData(const char* data) = 0;
    };

    class SqlRepository : public IRepository {
    public:
        SqlRepository(DotNetDupe::System::SmartPointer<IDatabaseConnection> spConn) : m_spConn(spConn) {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Constructed.");
        }
        ~SqlRepository() override {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Destructed.");
        }
        void SaveData(const char* data) override {
            DotNetDupe::System::Console::Write("  [SqlRepository] Saving data: ");
            DotNetDupe::System::Console::WriteLine(data);
            m_spConn->Query("INSERT INTO Data VALUES (...)");
        }
    private:
        DotNetDupe::System::SmartPointer<IDatabaseConnection> m_spConn;
    };
}

void DemonstrateDependencyInjection() {
    using namespace DotNetDupe::System;
    using namespace DotNetDupe::Extensions::DependencyInjection;
    using namespace DependencyInjectionDemo;

    Console::WriteLine("\n=== Dependency Injection Demonstration ===");
    try {
        ServiceCollection services;

        // Register IDatabaseConnection as Scoped
        services.AddScoped<IDatabaseConnection, SqlDatabaseConnection>();

        // Register IRepository as Transient, using a lambda factory to inject IDatabaseConnection
        services.AddTransient<IRepository>([](const SmartPointer<IServiceProvider>& sp) {
            auto spConn = sp->GetRequiredService<IDatabaseConnection>();
            return SmartPointer<Object>(SmartPointer<SqlRepository>::NewShared(spConn));
        });

        Console::WriteLine("Building ServiceProvider...");
        auto spProvider = services.BuildServiceProvider();

        Console::WriteLine("\n--- Scope 1 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 1...");
            auto spRepo1 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo1->SaveData("User1");

            Console::WriteLine("Resolving repository in Scope 1 again...");
            auto spRepo2 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo2->SaveData("User2");
        } // Scope 1 disposed, SqlDatabaseConnection should be disposed

        Console::WriteLine("\n--- Scope 2 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 2...");
            auto spRepo = spScopedProvider->GetRequiredService<IRepository>();
            spRepo->SaveData("User3");
        } // Scope 2 disposed, SqlDatabaseConnection should be disposed

    } catch (const BasicException<char>& ex) {
        Console::Write("Error during Dependency Injection demonstration: ");
        Console::WriteLine(ex.What());
    }
    Console::WriteLine("==========================================");
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
    DemonstrateHttp();
    DemonstrateHttps();
    DemonstrateJwt();
    DemonstrateDependencyInjection();
    
    Console::WriteLine("\n--- Demonstration Complete ---");
    Console::WriteLine("Press Enter to exit...");
    Console::ReadLine();
    return 0;
}
