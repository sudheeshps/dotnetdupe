### class `SystemMetrics` & `ActiveUserSession`

Provides real-time system resource usage telemetry (CPU, Memory, Disk Read/Write Bytes, Network, Top Processes) and active user session metrics.

---

#### Structs

##### `RealTimeSystemInfo`
- `double dCpuUsagePercent`
- `double dMemoryUsagePercent`
- `unsigned long long uMemoryTotalBytes` (total physical memory in raw bytes)
- `unsigned long long uMemoryUsedBytes` (used physical memory in raw bytes)
- `unsigned long long uDiskReadBytes` (system-wide total disk read transfer in raw bytes)
- `unsigned long long uDiskWriteBytes` (system-wide total disk write transfer in raw bytes)
- `double dNetworkUsageMbps`
- `List<ProcessResourceInfo> lstTopProcesses`

##### `ProcessResourceInfo`
- `int iProcessId`
- `String sName`
- `String sPath`
- `String sCommandLine` (full command line arguments)
- `double dCpuUsagePercent` (differential real-time CPU %)
- `long long lMemoryUsageBytes` (raw working set memory in bytes; `-1` if unqueryable)
- `long long lDiskReadBytes` (cumulative disk read transfer in raw bytes; `-1` if unqueryable)
- `long long lDiskWriteBytes` (cumulative disk write transfer in raw bytes; `-1` if unqueryable)

---

## Code Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    // Query system hardware metrics
    RealTimeSystemInfo metrics = SystemMetrics::GetSystemMetrics();
    Console::WriteLine("Disk Read Bytes: " + Convert::ToString(metrics.uDiskReadBytes));
    Console::WriteLine("Disk Write Bytes: " + Convert::ToString(metrics.uDiskWriteBytes));

    // Query active user sessions
    auto sessions = ActiveUserSession::GetAllSessions();
    Console::WriteLine("Sessions: " + Convert::ToString(sessions.GetCount()));
    return 0;
}
```
