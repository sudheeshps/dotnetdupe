# SystemMetrics

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/SystemMetrics.h"`

Provides system-wide and per-process hardware telemetry metrics including CPU %, Memory load, Disk I/O bytes, Network bandwidth/sockets, Top-consuming process ranking, and Windows/Linux service enumeration.

---

## Enums & Data Structures

### `SystemResource` Enum
```cpp
enum class SystemResource {
    Cpu,
    Memory,
    Disk,
    Network
};
```

### `MemoryInfo` Struct
- `double dMemoryUsagePercent`: Memory load percentage (0.0 to 100.0%).
- `unsigned long long uMemoryTotalBytes`: Total physical memory installed.
- `unsigned long long uMemoryUsedBytes`: Total physical memory currently in use.
- `long long lPrivateBytes`: Private memory committed by the process.
- `long long lPhysicalMemoryBytes`: Working set memory used by the process.

### `DiskInfo` Struct
- `long long lDiskReadBytes`: Total bytes read from disk.
- `long long lDiskWriteBytes`: Total bytes written to disk.

### `NetworkUsageInfo` Struct
- `long long lNetworkReadBytes`: Total inbound network bytes.
- `long long lNetworkWriteBytes`: Total outbound network bytes.

### `NetworkConnectionInfo` Struct
- `String sLocalAddress`: Local IP address.
- `int iLocalPort`: Local binding port.
- `String sRemoteAddress`: Remote peer IP address.
- `int iRemotePort`: Remote peer port.
- `String sState`: TCP connection state (`"ESTABLISHED"`, `"LISTEN"`, `"TIME_WAIT"`, etc.).

### `ProcessNetworkConnectionInfo` Struct
- `List<int> lstOpenPorts`: Active listening TCP/UDP ports owned by the process.
- `List<NetworkConnectionInfo> lstConnections`: List of open socket connections.
- `bool bHasEstablishedInboundConnection`: Whether an active established connection exists.

### `ProcessInfo` Struct
- `int iProcessId`: Process ID (PID).
- `int iSessionId`: User session ID.
- `String sName`: Process name (e.g. `"svchost.exe"`).
- `String sPath`: Executable path on disk.
- `String sCommandLine`: Command-line arguments.
- `double dCpuUsagePercent`: CPU load %.
- `MemoryInfo memory`, `DiskInfo disk`, `NetworkUsageInfo network`.

### `ServiceInfo` Struct
- `String sServiceName`: Service short name.
- `String sDisplayName`: Service friendly display name.
- `String sStatus`: Service state (`"Running"`, `"Stopped"`, etc.).
- `String sStartType`: Service startup configuration (`"Automatic"`, `"Manual"`, `"Disabled"`).
- `int iProcessId`: PID hosting the service.

---

## Static Telemetry APIs

### System-Wide Metrics
- `static double GetSystemCpuUsage()`: Returns total system CPU load percentage.
- `static MemoryInfo GetSystemMemoryUsage()`: Returns system memory stats.
- `static DiskInfo GetSystemDiskUsage()`: Returns aggregate system disk I/O throughput.
- `static double GetSystemNetworkUsage()`: Returns total system network bandwidth in Mbps.

### Process-Specific Metrics
- `static String GetProcessCommandLine(const String& sProcessName)`: Retrieves the complete command line arguments for the named process.
- `static MemoryInfo GetProcessMemoryUsage(const String& sProcessName)`: Retrieves memory statistics for the named process.
- `static DiskInfo GetProcessDiskUsage(const String& sProcessName)`: Retrieves disk read/write throughput for the named process.
- `static NetworkUsageInfo GetProcessNetworkUsage(const String& sProcessName)`: Retrieves network throughput for the named process.
- `static List<int> GetProcessNetworkPort(const String& sProcessName)`: Retrieves listening ports opened by the process.
- `static ProcessNetworkConnectionInfo GetProcessNetworkInfo(const String& sProcessName)`: Retrieves active TCP socket connection tables for the process.

### Process Enumeration & Ranking
- `static List<ProcessInfo> GetTopProcesses(SystemResource eResource, int iCount)`: Returns the top `iCount` processes consuming the highest CPU, Memory, Disk, or Network.
- `static List<ProcessInfo> GetAllProcesses(int iSessionId = -1)`: Enumerates all active processes across the system or within a specific session.
- `static List<ServiceInfo> GetAllServices()`: Enumerates all installed system services.

---

---

## Progressive Process Streaming (`ProcessStreamer`)

For non-blocking, two-tier progressive telemetry streaming without freezing UI threads or stalling network endpoints, use `SystemMetrics::CreateProcessStreamer()` or `SystemMetrics::EnumerateProcessesAsync()`. See the dedicated [ProcessStreamer documentation](ProcessStreamer.md) for detailed usage patterns.

```cpp
SystemMetrics::EnumerateProcessesAsync([](const ProcessInfo& proc) {
    Console::WriteLine("Discovered PID: {0} ({1})", proc.iProcessId, proc.sName);
});
```

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    Console::WriteLine("--- System Telemetry ---");
    Console::WriteLine("System CPU Load:    {0:F1} %", SystemMetrics::GetSystemCpuUsage());

    MemoryInfo mem = SystemMetrics::GetSystemMemoryUsage();
    Console::WriteLine("Memory In-Use:      {0:F1} % ({1} MB / {2} MB)", 
        mem.dMemoryUsagePercent, 
        mem.uMemoryUsedBytes / (1024 * 1024), 
        mem.uMemoryTotalBytes / (1024 * 1024));

    Console::WriteLine("\n--- Top 3 CPU Consuming Processes ---");
    auto topCpu = SystemMetrics::GetTopProcesses(SystemResource::Cpu, 3);
    for (int i = 0; i < topCpu.GetCount(); ++i) {
        Console::WriteLine(" [{0}] PID {1}: {2} ({3:F1}% CPU)", 
            i + 1, topCpu[i].iProcessId, topCpu[i].sName, topCpu[i].dCpuUsagePercent);
    }

    return 0;
}
```

