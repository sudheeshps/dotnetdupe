# ProcessStreamer, ProcessStreamOptions &amp; ProcessEventArgs

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/ProcessStreamer.h"`, `#include "System/Diagnostics/ProcessStreamOptions.h"`

Provides non-blocking, two-tier progressive telemetry streaming and event-driven observable process enumeration using idiomatic C# .NET `EventHandler` delegates for desktop UIs, real-time WebSocket backend services, and monitoring daemons.

---

## 1. Architectural Overview

Traditional process enumeration (`SystemMetrics::GetAllProcesses()`) is synchronous and can block the caller thread for 1.5–3.0 seconds to perform deep handle queries, CPU delta calculations, and TCP connection table inspections across all running processes.

`ProcessStreamer` solves this using a **two-tier streaming model**:

1. **Tier 1 (Fast Discovery, < 5ms)**: Instantly enumerates basic metadata (PID, Name, Session ID, Executable Path, and Working Set RAM) with `lstOpenPorts` empty and fires `ProcessDiscovered` and `BatchReady` events in configurable micro-batches for instantaneous UI rendering.
2. **Tier 2 (Progressive Metric Enrichment)**: Asynchronously samples deep metrics (CPU %, Disk I/O bytes, Network bandwidth, open listening TCP/UDP ports in `lstOpenPorts`, active socket connections in `lstConnections`, and established connection state in `bHasEstablishedConnection`) in the background and emits `ProcessUpdated` events.
3. **Cancellation & Throttling**: Streams can be cancelled dynamically via `Cancel()` or configured with custom batch sizes and flush intervals.

---

## 2. Enums & Configuration

### `ProcessMetricsDetail` Enum
```cpp
enum class ProcessMetricsDetail {
    FastDiscoveryOnly = 0, // Tier 1 only: Fast initial snapshot (<5ms)
    Progressive = 1,       // Tier 1 immediately, followed by Tier 2 deep metrics
    FullSynchronized = 2   // Deep metrics calculated before emission
};
```

### `ProcessStreamOptions` Struct
```cpp
struct ProcessStreamOptions {
    ProcessMetricsDetail eDetailLevel; // Default: Progressive
    int iBatchSize;                   // Default: 25 items per batch
    int iBatchIntervalMs;             // Default: 50ms flush interval
    int iSessionId;                   // Default: -1 (all sessions)
    bool bIncludeNetworkInfo;         // Default: true (open ports & TCP connections)
};
```

---

## 3. EventArgs Classes

### `ProcessEventArgs`
Carries a single discovered or updated `ProcessInfo` object.
```cpp
class ProcessEventArgs : public EventArgs {
public:
    const ProcessInfo& GetProcess() const;
};
```

### `ProcessBatchEventArgs`
Carries a micro-batch chunk of `List<ProcessInfo>`.
```cpp
class ProcessBatchEventArgs : public EventArgs {
public:
    const Collections::Generic::List<ProcessInfo>& GetBatch() const;
};
```

### `ProcessStreamErrorEventArgs`
Carries error information when streaming encounters an unrecoverable exception.
```cpp
class ProcessStreamErrorEventArgs : public EventArgs {
public:
    String GetErrorMessage() const;
};
```

---

## 4. `ProcessStreamer` Class

### Syntax
```cpp
class ProcessStreamer : public virtual Object;
```

### Multicast Events
- `EventHandler<ProcessEventArgs> ProcessDiscovered`: Fires when a new process is discovered in Tier-1.
- `EventHandler<ProcessBatchEventArgs> BatchReady`: Fires when a micro-batch of processes is ready.
- `EventHandler<ProcessEventArgs> ProcessUpdated`: Fires when Tier-2 deep metrics (CPU, disk, ports) are enriched.
- `EventHandler<> Completed`: Fires when streaming completes.
- `EventHandler<ProcessStreamErrorEventArgs> Error`: Fires when a streaming error occurs.

### Methods
- `explicit ProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions())`: Initializes streamer.
- `void Start()`: Starts the background streaming worker thread.
- `void Cancel()`: Halts the in-flight stream and joins the background thread.
- `bool IsRunning() const`: Returns whether the streamer is actively running.
- `ProcessStreamOptions GetOptions() const`: Returns configured options.

---

## 5. Usage Example

```cpp
#include "System/Console.h"
#include "System/Convert.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Threading/AutoResetEvent.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

int main() {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::Progressive;
    options.bIncludeNetworkInfo = true;
    options.iBatchSize = 10;

    auto pStreamer = SystemMetrics::CreateProcessStreamer(options);
    AutoResetEvent evtDone(false);

    // Multicast subscription for discovered processes
    pStreamer->ProcessDiscovered += [](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        const auto& proc = e.GetProcess();
        Console::WriteLine(String("  [Discovered] PID: ") + Convert::ToString(proc.iProcessId) + " | " + proc.sName);
    };

    // Subscription for enriched metrics
    pStreamer->ProcessUpdated += [](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        const auto& proc = e.GetProcess();
        Console::WriteLine(String("  [Enriched] PID: ") + Convert::ToString(proc.iProcessId) + " | Ports count: " + Convert::ToString(proc.lstOpenPorts.GetCount()));
    };

    // Subscription for completion
    pStreamer->Completed += [&evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        Console::WriteLine("Process telemetry streaming completed.");
        evtDone.Set();
    };

    pStreamer->Start();
    evtDone.WaitOne();

    return 0;
}
```
