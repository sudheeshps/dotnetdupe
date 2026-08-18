# ProcessStreamer &amp; ProcessStreamOptions

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/ProcessStreamer.h"`, `#include "System/Diagnostics/ProcessStreamOptions.h"`

Provides non-blocking, two-tier progressive telemetry streaming and event-driven observable process enumeration for desktop UIs, real-time WebSocket backend services, and monitoring daemons.

---

## 1. Architectural Overview

Traditional process enumeration (`SystemMetrics::GetAllProcesses()` or `GetTopProcesses()`) is synchronous and can block the caller thread for 1.5–3.0 seconds to perform deep handle queries, CPU delta calculations, and TCP connection table inspections across all running processes.

`ProcessStreamer` solves this using a **two-tier streaming model**:

1. **Tier 1 (Fast Discovery, $<5\text{ms}$)**: Instantly enumerates basic metadata (PID, Name, Session ID, Executable Path, and Working Set RAM) and fires `OnProcess` and `OnBatch` events in configurable micro-batches.
2. **Tier 2 (Progressive Metric Enrichment)**: Asynchronously samples deep metrics (CPU %, Disk I/O bytes, Network bandwidth, open TCP/UDP ports) in the background and emits `OnProcessUpdated` events.
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

## 3. Interfaces & Observable Classes

### `IProcessObserver` Interface
```cpp
class IProcessObserver : public virtual Object {
public:
    virtual void OnProcessDiscovered(const ProcessInfo& proc) = 0;
    virtual void OnBatchReady(const Collections::Generic::List<ProcessInfo>& lstBatch) = 0;
    virtual void OnProcessUpdated(const ProcessInfo& proc) = 0;
    virtual void OnCompleted() = 0;
    virtual void OnError(const Exception& ex) = 0;
};
```

### `ProcessStreamer` Class
- `explicit ProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions())`: Initializes streamer.
- `void OnProcess(const Action<const ProcessInfo&>& fnOnProcess)`: Subscribes to single-item discovery events.
- `void OnBatch(const Action<const Collections::Generic::List<ProcessInfo>&>& fnOnBatch)`: Subscribes to micro-batch events.
- `void OnProcessUpdated(const Action<const ProcessInfo&>& fnOnUpdated)`: Subscribes to deep metric enrichment events.
- `void OnCompleted(const Action<>& fnOnCompleted)`: Subscribes to completion event.
- `void OnError(const Action<const Exception&>& fnOnError)`: Subscribes to error event.
- `void Subscribe(const SmartPointer<IProcessObserver>& pObserver)`: Subscribes an `IProcessObserver` instance.
- `void Start()`: Starts the background streaming worker thread.
- `void Cancel()`: Halts the in-flight stream and joins the background thread.
- `bool IsRunning() const`: Returns whether the streamer is actively running.
- `ProcessStreamOptions GetOptions() const`: Returns configured options.

---

## 4. Usage Examples

### Example A: Streaming Over WebSockets to a Web UI

```cpp
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamer.h"
#include "WebAppCore/WebSockets/WebSocketContext.h"
#include "System/JsonSerializer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::WebAppCore::WebSockets;

class ProcessTelemetryHandler : public IWebSocketHandler {
public:
    void OnConnected(SmartPointer<WebSocketContext> pContext) override {
        ProcessStreamOptions options;
        options.eDetailLevel = ProcessMetricsDetail::Progressive;
        options.iBatchSize = 20;

        auto pStreamer = SystemMetrics::CreateProcessStreamer(options);

        pStreamer->OnBatch([pContext](const Collections::Generic::List<ProcessInfo>& lstBatch) {
            String sJson = JsonSerializer::Serialize(lstBatch);
            pContext->GetWebSocket()->SendAsync(String("{\"type\":\"batch\",\"data\":") + sJson + "}");
        });

        pStreamer->OnProcessUpdated([pContext](const ProcessInfo& proc) {
            String sJson = JsonSerializer::Serialize(proc);
            pContext->GetWebSocket()->SendAsync(String("{\"type\":\"update\",\"data\":") + sJson + "}");
        });

        pStreamer->OnCompleted([pContext]() {
            pContext->GetWebSocket()->SendAsync("{\"type\":\"completed\"}");
        });

        pStreamer->Start();
    }

    void OnMessage(SmartPointer<WebSocketContext> pContext, const String& message) override {}
    void OnDisconnected(SmartPointer<WebSocketContext> pContext) override {}
};
```

---

### Example B: Rapid Desktop / Console UI Population

```cpp
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void StreamProcesses() {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);

    pStreamer->OnProcess([](const ProcessInfo& proc) {
        Console::WriteLine(String::Format("PID: {0,-6} | Name: {1,-25} | RAM: {2} MB",
            proc.iProcessId,
            proc.sName,
            proc.memory.lPhysicalMemoryBytes / (1024 * 1024)));
    });

    pStreamer->OnCompleted([]() {
        Console::WriteLine("Process discovery completed.");
    });

    pStreamer->Start();
}
```
