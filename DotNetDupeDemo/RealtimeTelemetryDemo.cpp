#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"
#include "System/Diagnostics/Process.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateRealtimeTelemetry() {
    Console::WriteLine("\n--- Realtime Telemetry & Session Info Demonstration ---");

    // 1. Query Hardware System Metrics
    MemoryInfo mem = SystemMetrics::GetSystemMemoryUsage();
    double dCpu = SystemMetrics::GetSystemCpuUsage();
    DiskInfo disk = SystemMetrics::GetSystemDiskUsage();
    Console::WriteLine("Memory Load: " + Convert::ToString(static_cast<int>(mem.dMemoryUsagePercent)) + "% (" + Convert::ToString(static_cast<long long>(mem.uMemoryUsedBytes / (1024 * 1024))) + " MB / " + Convert::ToString(static_cast<long long>(mem.uMemoryTotalBytes / (1024 * 1024))) + " MB)");
    Console::WriteLine("CPU Usage: " + Convert::ToString(static_cast<int>(dCpu)) + "%");
    Console::WriteLine("Disk Read Bytes: " + Convert::ToString(static_cast<long long>(disk.lDiskReadBytes)) + " Bytes");
    Console::WriteLine("Disk Write Bytes: " + Convert::ToString(static_cast<long long>(disk.lDiskWriteBytes)) + " Bytes");

    // 2. Discover Running Processes via Process::GetProcesses()
    auto arrProcesses = Process::GetProcesses();
    Console::WriteLine("Total Active Processes (Process::GetProcesses): " + Convert::ToString(arrProcesses.GetLength()));

    // 3. On-Demand Process Enrichment
    if (arrProcesses.GetLength() > 0 && !arrProcesses[0].IsNull()) {
        ProcessInfo sampleProc;
        sampleProc.iProcessId = arrProcesses[0]->GetId();
        sampleProc.sName = arrProcesses[0]->GetProcessName();
        SystemMetrics::EnrichProcessInfo(sampleProc, true);
        Console::WriteLine("Sample Process Enriched: " + sampleProc.sName + " (PID: " + Convert::ToString(sampleProc.iProcessId) + ")");
        Console::WriteLine(" - RAM: " + Convert::ToString(static_cast<long long>(sampleProc.memory.lPhysicalMemoryBytes / (1024 * 1024))) + " MB");
        Console::WriteLine(" - Open Ports: " + Convert::ToString(sampleProc.lstOpenPorts.GetCount()));
    }

    // 4. Query Services
    auto lstServices = SystemMetrics::GetAllServices();
    Console::WriteLine("Total Services Count: " + Convert::ToString(lstServices.GetCount()));

    // 5. Query Active & Total Sessions
    auto lstSessions = ActiveUserSession::GetAllSessions();
    Console::WriteLine("Active User Sessions Count: " + Convert::ToString(lstSessions.GetCount()));
    for (int i = 0; i < lstSessions.GetCount(); i++) {
        Console::WriteLine(" - Session [" + Convert::ToString(static_cast<int>(lstSessions[i].uSessionId)) + "] User: " + lstSessions[i].sUsername + " (" + lstSessions[i].sPrivilege + ")");
    }
}
