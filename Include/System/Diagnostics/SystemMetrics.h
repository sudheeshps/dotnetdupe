#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Action.h"
#include "System/Collections/Generic/List.h"
#include "System/Diagnostics/ProcessStreamOptions.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            class ProcessStreamer;

            /// \struct MemoryInfo
            /// \brief Snapshot of physical and virtual memory utilization for the system or a specific process.
            struct MemoryInfo {
                double dMemoryUsagePercent;             ///< Percentage of total physical memory currently in use (0.0 - 100.0).
                unsigned long long uMemoryTotalBytes;   ///< Total physical RAM installed on the host in bytes.
                unsigned long long uMemoryUsedBytes;    ///< Currently consumed physical RAM across all processes.
                long long lPrivateBytes;                ///< Private bytes allocated by the target process (-1 if unavailable).
                long long lPhysicalMemoryBytes;         ///< Working set size (physical RAM resident) for the target process.

                /// \brief Default constructor initializing default counters.
                MemoryInfo()
                    : dMemoryUsagePercent(0.0),
                      uMemoryTotalBytes(0ULL),
                      uMemoryUsedBytes(0ULL),
                      lPrivateBytes(-1),
                      lPhysicalMemoryBytes(-1) {}
            };

            /// \struct DiskInfo
            /// \brief Rate or aggregate count of disk read and write operations.
            struct DiskInfo {
                long long lDiskReadBytes;   ///< Disk read throughput in bytes per second or total transfer count.
                long long lDiskWriteBytes;  ///< Disk write throughput in bytes per second or total transfer count.

                /// \brief Default constructor initializing to unpopulated (-1).
                DiskInfo() : lDiskReadBytes(-1), lDiskWriteBytes(-1) {}
            };

            /// \struct NetworkUsageInfo
            /// \brief Network throughput counters for bytes transmitted and received.
            struct NetworkUsageInfo {
                long long lNetworkReadBytes;    ///< Incoming network throughput in bytes or octets.
                long long lNetworkWriteBytes;   ///< Outgoing network throughput in bytes or octets.

                /// \brief Default constructor initializing to unpopulated (-1).
                NetworkUsageInfo() : lNetworkReadBytes(-1), lNetworkWriteBytes(-1) {}
            };

            /// \struct NetworkConnectionInfo
            /// \brief Telemetry describing an active TCP or UDP socket endpoint.
            struct NetworkConnectionInfo {
                String sLocalAddress;   ///< Local IPv4 or IPv6 IP address.
                int iLocalPort;         ///< Local listening or bound port.
                String sRemoteAddress;  ///< Remote peer IP address.
                int iRemotePort;        ///< Remote peer port.
                String sState;          ///< Socket connection state (e.g. "ESTABLISHED", "LISTEN", "TIME_WAIT").

                /// \brief Default constructor.
                NetworkConnectionInfo() : iLocalPort(0), iRemotePort(0) {}
            };

            /// \struct ProcessNetworkConnectionInfo
            /// \brief Collection of open ports and active connections attributed to a specific process.
            struct ProcessNetworkConnectionInfo {
                Collections::Generic::List<int> lstOpenPorts;                       ///< List of TCP/UDP ports bound or in LISTEN state.
                Collections::Generic::List<NetworkConnectionInfo> lstConnections;    ///< Detailed active socket connections.
                bool bHasEstablishedInboundConnection;                              ///< True if process possesses at least one established connection.

                /// \brief Default constructor.
                ProcessNetworkConnectionInfo() : bHasEstablishedInboundConnection(false) {}
            };

            /// \struct ProcessInfo
            /// \brief Comprehensive telemetry snapshot for an operating system process.
            struct ProcessInfo {
                int iProcessId;                                                 ///< Operating system process identifier (PID).
                int iSessionId;                                                 ///< Terminal Services session ID.
                String sName;                                                   ///< Process executable name without path.
                String sPath;                                                   ///< Full filesystem path to the executable image.
                String sCommandLine;                                            ///< Full command-line arguments used to spawn the process.
                double dCpuUsagePercent;                                        ///< Process CPU consumption percentage (0.0 - 100.0).
                MemoryInfo memory;                                              ///< Memory allocation statistics.
                DiskInfo disk;                                                  ///< Disk I/O telemetry.
                NetworkUsageInfo network;                                       ///< Network traffic metrics.
                Collections::Generic::List<int> lstOpenPorts;                   ///< Open listening ports.
                Collections::Generic::List<NetworkConnectionInfo> lstConnections;///< Active socket connections.
                bool bHasEstablishedConnection;                                 ///< True if any connection is established.

                /// \brief Default constructor.
                ProcessInfo()
                    : iProcessId(0),
                      iSessionId(0),
                      dCpuUsagePercent(0.0),
                      bHasEstablishedConnection(false) {}
            };

            /// \struct ServiceInfo
            /// \brief Telemetry snapshot for a system background service or daemon.
            struct ServiceInfo {
                String sServiceName;    ///< Internal service key identifier.
                String sDisplayName;    ///< Human-readable friendly display name.
                String sStatus;         ///< Current service state (e.g. "Running", "Stopped", "Paused").
                String sStartType;      ///< Startup behavior (e.g. "Automatic", "Manual", "Disabled").
                int iProcessId;         ///< Process ID hosting the service (0 if stopped).

                /// \brief Default constructor.
                ServiceInfo() : iProcessId(0) {}
            };

            /// \class SystemMetrics
            /// \brief Provides cross-platform diagnostic counters for CPU, memory, disk, network, processes, and services.
            ///
            /// Interfaces directly with native platform APIs: on Windows, uses PDH (Performance Data Helper),
            /// PSAPI, IPHLPAPI, Winternl NT queries, and Service Control Manager; on Linux, inspects `/proc`
            /// filesystems (`/proc/stat`, `/proc/meminfo`, `/proc/net/dev`, `/proc/[pid]/io`).
            class SystemMetrics : public Object {
            public:
                /// \brief Initializes a new instance of SystemMetrics.
                DOTNETDUPE_API SystemMetrics();

                /// \brief Virtual destructor.
                DOTNETDUPE_API virtual ~SystemMetrics();

                // System-wide metrics

                /// \brief Retrieves total and consumed physical memory metrics for the entire machine.
                /// \return MemoryInfo structure.
                DOTNETDUPE_API static MemoryInfo GetSystemMemoryUsage();

                /// \brief Computes total system CPU usage percentage across all processors.
                /// \return Double representing total CPU load (0.0 - 100.0).
                DOTNETDUPE_API static double GetSystemCpuUsage();

                /// \brief Retrieves total disk read and write transfer rates for the system.
                /// \return DiskInfo structure.
                DOTNETDUPE_API static DiskInfo GetSystemDiskUsage();

                /// \brief Computes total network traffic throughput across all operational interfaces in Megabits per second.
                /// \return Double representing aggregated throughput in Mbps.
                DOTNETDUPE_API static double GetSystemNetworkUsage();

                // Process-specific metrics (by process name)

                /// \brief Queries the full command line used to launch a process by name.
                /// \param sProcessName The process executable name.
                /// \return The command line string.
                DOTNETDUPE_API static String GetProcessCommandLine(const String& sProcessName);

                /// \brief Queries memory allocation details for a process by name.
                /// \param sProcessName The process executable name.
                /// \return MemoryInfo structure.
                DOTNETDUPE_API static MemoryInfo GetProcessMemoryUsage(const String& sProcessName);

                /// \brief Queries disk transfer throughput for a process by name.
                /// \param sProcessName The process executable name.
                /// \return DiskInfo structure.
                DOTNETDUPE_API static DiskInfo GetProcessDiskUsage(const String& sProcessName);

                /// \brief Queries network transfer metrics for a process by name.
                /// \param sProcessName The process executable name.
                /// \return NetworkUsageInfo structure.
                DOTNETDUPE_API static NetworkUsageInfo GetProcessNetworkUsage(const String& sProcessName);

                /// \brief Retrieves open listening ports bound by a process by name.
                /// \param sProcessName The process executable name.
                /// \return List of port numbers.
                DOTNETDUPE_API static Collections::Generic::List<int> GetProcessNetworkPort(const String& sProcessName);

                /// \brief Queries detailed socket connections for a process by name.
                /// \param sProcessName The process executable name.
                /// \return ProcessNetworkConnectionInfo structure.
                DOTNETDUPE_API static ProcessNetworkConnectionInfo GetProcessNetworkInfo(const String& sProcessName);

                // Process-specific metrics by PID

                /// \brief Retrieves open listening ports bound by a process by PID.
                /// \param iProcessId Target process identifier.
                /// \return List of port numbers.
                DOTNETDUPE_API static Collections::Generic::List<int> GetProcessNetworkPort(int iProcessId);

                /// \brief Queries socket connections for a process by PID.
                /// \param iProcessId Target process identifier.
                /// \return ProcessNetworkConnectionInfo structure.
                DOTNETDUPE_API static ProcessNetworkConnectionInfo GetProcessNetworkInfo(int iProcessId);

                /// \brief Enriches an existing ProcessInfo instance with CPU, memory, disk, and network telemetry.
                /// \param proc Target ProcessInfo to populate.
                /// \param bIncludeNetwork True to perform socket table scans.
                /// \throws DotNetDupe::System::ArgumentException If process ID is invalid.
                DOTNETDUPE_API static void EnrichProcessInfo(ProcessInfo& proc, bool bIncludeNetwork = true);

                // Progressive Streaming APIs

                /// \brief Creates a background process streaming engine configured with options.
                /// \param options Configuration settings for batching and depth.
                /// \return Smart pointer to initialized ProcessStreamer.
                DOTNETDUPE_API static SmartPointer<ProcessStreamer> CreateProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions());

                /// \brief Asynchronously discovers processes on the host using fast discovery mode.
                /// \param fnOnProcess Callback invoked for each discovered process.
                /// \param fnOnComplete Optional callback invoked when enumeration finishes.
                DOTNETDUPE_API static void EnumerateProcessesAsync(const Action<const ProcessInfo&>& fnOnProcess, const Action<>& fnOnComplete = nullptr);

                // Service enumeration API

                /// \brief Enumerates all registered system background services and their current execution status.
                /// \return List of ServiceInfo structures.
                DOTNETDUPE_API static Collections::Generic::List<ServiceInfo> GetAllServices();

            private:
#if defined(_WIN32)
                static void* OpenProcessByName(const String& sProcessName, unsigned long dwDesiredAccess, int& iOutProcessId);
                static String ReadProcessCommandLineHandle(void* hProc);
                static MemoryInfo ReadProcessMemoryHandle(void* hProc);
                static DiskInfo ReadProcessDiskHandle(void* hProc, const String& sProcessName);
                static NetworkUsageInfo ReadProcessNetworkHandle(void* hProc, const String& sProcessName);
                static Collections::Generic::List<int> ReadProcessNetworkPortInternal(int iProcessId);
                static ProcessNetworkConnectionInfo ReadProcessNetworkInfoInternal(int iProcessId);
#else
                static int FindPidByName(const String& sProcessName);
                static MemoryInfo ReadLinuxProcessMemory(int iPid);
                static DiskInfo ReadLinuxProcessDisk(int iPid);
                static NetworkUsageInfo ReadLinuxProcessNetwork(int iPid);
                static Collections::Generic::List<int> ReadLinuxProcessPorts(int iPid);
                static ProcessNetworkConnectionInfo ReadLinuxProcessNetworkInfo(int iPid);
#endif
            };

        }
    }
}
