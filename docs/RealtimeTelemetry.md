### class `SystemMetrics` & `ActiveUserSession`

Provides granular real-time system hardware telemetry (Memory, CPU, Disk, Network), process and service enumeration, and process-level handle-based resource metrics.

---

#### Enums & Structs

- `MemoryInfo`: `double dMemoryUsagePercent`, `unsigned long long uMemoryTotalBytes`, `unsigned long long uMemoryUsedBytes`, `long long lPrivateBytes`, `long long lPhysicalMemoryBytes` (working set)
- `DiskInfo`: `long long lDiskReadBytes`, `long long lDiskWriteBytes`
- `NetworkUsageInfo`: `long long lNetworkReadBytes`, `long long lNetworkWriteBytes`
- `ProcessNetworkConnectionInfo`: `List<int> lstOpenPorts`, `List<NetworkConnectionInfo> lstConnections`, `bool bHasEstablishedInboundConnection`
- `ProcessInfo`: `iProcessId`, `iSessionId`, `sName`, `sPath`, `sCommandLine`, `dCpuUsagePercent`, `MemoryInfo memory`, `DiskInfo disk`, `NetworkUsageInfo network`
- `ServiceInfo`: `sServiceName`, `sDisplayName`, `sStatus`, `sStartType`, `iProcessId`

---

#### Methods

##### System-Wide Metrics
- `MemoryInfo SystemMetrics::GetSystemMemoryUsage()`
- `double SystemMetrics::GetSystemCpuUsage()`
- `DiskInfo SystemMetrics::GetSystemDiskUsage()`
- `double SystemMetrics::GetSystemNetworkUsage()`

##### Process-Specific Metrics (Handle-Based)
- `String SystemMetrics::GetProcessCommandLine(const String& sProcessName)`
- `MemoryInfo SystemMetrics::GetProcessMemoryUsage(const String& sProcessName)`
- `DiskInfo SystemMetrics::GetProcessDiskUsage(const String& sProcessName)`
- `NetworkUsageInfo SystemMetrics::GetProcessNetworkUsage(const String& sProcessName)`
- `List<int> SystemMetrics::GetProcessNetworkPort(const String& sProcessName)`
- `ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(const String& sProcessName)`
- `List<int> SystemMetrics::GetProcessNetworkPort(int iProcessId)`
- `ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(int iProcessId)`
- `void SystemMetrics::EnrichProcessInfo(ProcessInfo& proc, bool bIncludeNetwork = true)`

##### Process & Service Listing
- `List<ProcessInfo> SystemMetrics::GetAllProcesses(int iSessionId = -1)`
- `List<ServiceInfo> SystemMetrics::GetAllServices()`

---

## Code Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    // Query system-wide metrics
    MemoryInfo mem = SystemMetrics::GetSystemMemoryUsage();
    double dCpu = SystemMetrics::GetSystemCpuUsage();
    DiskInfo disk = SystemMetrics::GetSystemDiskUsage();

    // Process specific handle-based metrics
    String sCmd = SystemMetrics::GetProcessCommandLine(String("svchost.exe"));
    ProcessNetworkConnectionInfo netInfo = SystemMetrics::GetProcessNetworkInfo(String("svchost.exe"));

    // Process & service listing APIs
    auto lstAllProc = SystemMetrics::GetAllProcesses(-1);
    auto lstServices = SystemMetrics::GetAllServices();

    return 0;
}
```
