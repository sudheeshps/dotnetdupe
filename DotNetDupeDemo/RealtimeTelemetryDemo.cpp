#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateRealtimeTelemetry() {
    Console::WriteLine("\n--- Realtime Telemetry & Session Info Demonstration ---");

    // 1. Query Hardware System Metrics
    MemoryInfo mem = SystemMetrics::GetSystemMemoryUsage();
    double dCpu = SystemMetrics::GetSystemCpuUsage();
    DiskInfo disk = SystemMetrics::GetSystemDiskUsage();
    Console::WriteLine("Memory Load: " + Convert::ToString(static_cast<int>(mem.dMemoryUsagePercent)) + "% (" + Convert::ToString(static_cast<long long>(mem.uMemoryUsedBytes)) + " Bytes / " + Convert::ToString(static_cast<long long>(mem.uMemoryTotalBytes)) + " Bytes)");
    Console::WriteLine("CPU Usage: " + Convert::ToString(static_cast<int>(dCpu)) + "%");
    Console::WriteLine("Disk Read Bytes: " + Convert::ToString(static_cast<long long>(disk.lDiskReadBytes)) + " Bytes");
    Console::WriteLine("Disk Write Bytes: " + Convert::ToString(static_cast<long long>(disk.lDiskWriteBytes)) + " Bytes");

    // 2. Query Processes & Services
    auto lstAllProcesses = SystemMetrics::GetAllProcesses(-1);
    Console::WriteLine("Total Processes Count: " + Convert::ToString(lstAllProcesses.GetCount()));

    auto lstServices = SystemMetrics::GetAllServices();
    Console::WriteLine("Total Services Count: " + Convert::ToString(lstServices.GetCount()));

    // 2. Query Active & Total Sessions
    auto lstSessions = ActiveUserSession::GetAllSessions();
    Console::WriteLine("Active User Sessions Count: " + Convert::ToString(lstSessions.GetCount()));
    for (int i = 0; i < lstSessions.GetCount(); i++) {
        Console::WriteLine(" - Session [" + Convert::ToString(static_cast<int>(lstSessions[i].uSessionId)) + "] User: " + lstSessions[i].sUsername + " (" + lstSessions[i].sPrivilege + ")");
    }
}
