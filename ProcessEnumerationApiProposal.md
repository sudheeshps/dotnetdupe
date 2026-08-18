# DotNetDupe API Proposal: Progressive Process Enumeration & Telemetry Streaming

The full proposal document has been created and saved to the project documentation:
👉 [docs/ProcessEnumerationApiProposal.md](file:///d:/Personal/Projects/C++/smarteventviewer/docs/ProcessEnumerationApiProposal.md)

---

## Executive Summary

### 1. Problem: "Fire and Hang" Synchronous Batching
`DotNetDupe::System::Diagnostics::SystemMetrics::GetTopProcesses(resource, count)` performs full Win32 handle opening, CPU sampling deltas, PEB/WMI queries, and TCP connection table inspection for all 200–400 system processes synchronously on the calling thread, blocking execution for 1.5–3.0 seconds.

### 2. Proposed Solution: Streaming & Client-Side Filtering
1. **DotNetDupe Visitor / Async API**:
   - `SystemMetrics::EnumerateProcesses(const ProcessYieldHandler& onYield, const CancellationToken& token)`
   - `SystemMetrics::EnumerateProcessesAsync(...)`
   - `ProcessStreamer` observable class emitting `OnProcessFound` and `OnBatchReady`.
2. **Two-Tier Inspection**:
   - **Tier 1 (Fast, $<5\text{ms}$)**: PIDs, Names, Working Set RAM.
   - **Tier 2 (Deep, progressive)**: CPU deltas, Disk I/O, open ports.
3. **Client-Side Sorting**:
   - Web application receives all processes progressively over WebSocket/worker.
   - User dynamically sorts by CPU, Memory, Disk, Network, or Name in 0ms directly in the browser.
