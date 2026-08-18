#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

static void DemoFastProcessDiscovery() {
    Console::WriteLine("\n--- 1. Fast Tier-1 Process Discovery (<5ms) ---");
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 5;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SystemMetrics::CreateProcessStreamer(options);
    AutoResetEvent evtDone(false);

    pStreamer->OnProcess([](const ProcessInfo& proc) {
        Console::WriteLine(String(" [DISCOVERED] PID: ") + Convert::ToString(proc.iProcessId) + " | Name: " + proc.sName + " | RAM: " + Convert::ToString(proc.memory.lPhysicalMemoryBytes / (1024 * 1024)) + " MB");
    });
    pStreamer->OnCompleted([&evtDone]() {
        Console::WriteLine(" Fast Tier-1 discovery completed.");
        evtDone.Set();
    });

    pStreamer->Start();
    evtDone.WaitOne(5000);
}

static void DemoBatchStreaming() {
    Console::WriteLine("\n--- 2. Micro-Batch Streaming (Configurable Batch Size) ---");
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SystemMetrics::CreateProcessStreamer(options);
    AutoResetEvent evtDone(false);

    pStreamer->OnBatch([](const Collections::Generic::List<ProcessInfo>& lstBatch) {
        Console::WriteLine(String(" [BATCH] Received chunk of ") + Convert::ToString(lstBatch.GetCount()) + " processes.");
    });
    pStreamer->OnCompleted([&evtDone]() {
        Console::WriteLine(" Batch streaming completed.");
        evtDone.Set();
    });

    pStreamer->Start();
    evtDone.WaitOne(5000);
}

static void DemoAsyncConvenienceHelper() {
    Console::WriteLine("\n--- 3. Static EnumerateProcessesAsync Helper ---");
    AutoResetEvent evtDone(false);

    SystemMetrics::EnumerateProcessesAsync(
        [](const ProcessInfo& proc) {
            if (proc.iProcessId > 0 && proc.sName.Contains("host")) {
                Console::WriteLine(String(" Matched host process: ") + proc.sName + " (PID: " + Convert::ToString(proc.iProcessId) + ")");
            }
        },
        [&evtDone]() {
            Console::WriteLine(" Async enumeration helper completed.");
            evtDone.Set();
        });

    evtDone.WaitOne(5000);
}

void DemonstrateProcessStreamer() {
    Console::WriteLine("\n=======================================================");
    Console::WriteLine(" Progressive Process Enumerator (ProcessStreamer) Demo");
    Console::WriteLine("=======================================================");

    DemoFastProcessDiscovery();
    DemoBatchStreaming();
    DemoAsyncConvenienceHelper();
}
