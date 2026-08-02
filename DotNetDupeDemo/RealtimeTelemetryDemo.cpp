#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateRealtimeTelemetry() {
    Console::WriteLine("\n--- Realtime Telemetry & Session Info Demonstration ---");

    // 1. Query Hardware System Metrics
    RealTimeSystemInfo metrics = SystemMetrics::GetSystemMetrics();
    Console::WriteLine("Memory Load: " + Convert::ToString(static_cast<int>(metrics.dMemoryUsagePercent)) + "% (" + Convert::ToString(static_cast<long long>(metrics.uMemoryUsedBytes)) + " Bytes / " + Convert::ToString(static_cast<long long>(metrics.uMemoryTotalBytes)) + " Bytes)");
    Console::WriteLine("CPU Usage: " + Convert::ToString(static_cast<int>(metrics.dCpuUsagePercent)) + "%");
    Console::WriteLine("Disk Read Bytes: " + Convert::ToString(static_cast<long long>(metrics.uDiskReadBytes)) + " Bytes");
    Console::WriteLine("Disk Write Bytes: " + Convert::ToString(static_cast<long long>(metrics.uDiskWriteBytes)) + " Bytes");
    Console::WriteLine("Top Processes Count: " + Convert::ToString(metrics.lstTopProcesses.GetCount()));

    // 2. Query Active & Total Sessions
    auto lstSessions = ActiveUserSession::GetAllSessions();
    Console::WriteLine("Active User Sessions Count: " + Convert::ToString(lstSessions.GetCount()));
    for (int i = 0; i < lstSessions.GetCount(); i++) {
        Console::WriteLine(" - Session [" + Convert::ToString(static_cast<int>(lstSessions[i].uSessionId)) + "] User: " + lstSessions[i].sUsername + " (" + lstSessions[i].sPrivilege + ")");
    }
}
