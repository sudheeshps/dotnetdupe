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

            struct MemoryInfo {
                double dMemoryUsagePercent;
                unsigned long long uMemoryTotalBytes;
                unsigned long long uMemoryUsedBytes;
                long long lPrivateBytes;
                long long lPhysicalMemoryBytes; // Working Set

                MemoryInfo()
                    : dMemoryUsagePercent(0.0),
                      uMemoryTotalBytes(0ULL),
                      uMemoryUsedBytes(0ULL),
                      lPrivateBytes(-1),
                      lPhysicalMemoryBytes(-1) {}
            };

            struct DiskInfo {
                long long lDiskReadBytes;
                long long lDiskWriteBytes;

                DiskInfo() : lDiskReadBytes(-1), lDiskWriteBytes(-1) {}
            };

            struct NetworkUsageInfo {
                long long lNetworkReadBytes;
                long long lNetworkWriteBytes;

                NetworkUsageInfo() : lNetworkReadBytes(-1), lNetworkWriteBytes(-1) {}
            };

            struct NetworkConnectionInfo {
                String sLocalAddress;
                int iLocalPort;
                String sRemoteAddress;
                int iRemotePort;
                String sState; // e.g. "ESTABLISHED", "LISTEN", "TIME_WAIT", etc.

                NetworkConnectionInfo() : iLocalPort(0), iRemotePort(0) {}
            };

            struct ProcessNetworkConnectionInfo {
                Collections::Generic::List<int> lstOpenPorts;
                Collections::Generic::List<NetworkConnectionInfo> lstConnections;
                bool bHasEstablishedInboundConnection;

                ProcessNetworkConnectionInfo() : bHasEstablishedInboundConnection(false) {}
            };

            struct ProcessInfo {
                int iProcessId;
                int iSessionId;
                String sName;
                String sPath;
                String sCommandLine;
                double dCpuUsagePercent;
                MemoryInfo memory;
                DiskInfo disk;
                NetworkUsageInfo network;
                Collections::Generic::List<int> lstOpenPorts;
                Collections::Generic::List<NetworkConnectionInfo> lstConnections;
                bool bHasEstablishedConnection;

                ProcessInfo()
                    : iProcessId(0),
                      iSessionId(0),
                      dCpuUsagePercent(0.0),
                      bHasEstablishedConnection(false) {}
            };

            struct ServiceInfo {
                String sServiceName;
                String sDisplayName;
                String sStatus; // e.g. "Running", "Stopped", "Paused", "StartPending", etc.
                String sStartType; // e.g. "Automatic", "Manual", "Disabled", etc.
                int iProcessId;

                ServiceInfo() : iProcessId(0) {}
            };

            class SystemMetrics : public Object {
            public:
                DOTNETDUPE_API SystemMetrics();
                DOTNETDUPE_API virtual ~SystemMetrics();

                // System-wide metrics
                DOTNETDUPE_API static MemoryInfo GetSystemMemoryUsage();
                DOTNETDUPE_API static double GetSystemCpuUsage();
                DOTNETDUPE_API static DiskInfo GetSystemDiskUsage();
                DOTNETDUPE_API static double GetSystemNetworkUsage();

                // Process-specific metrics (by process name, internally using handle-based APIs)
                DOTNETDUPE_API static String GetProcessCommandLine(const String& sProcessName);
                DOTNETDUPE_API static MemoryInfo GetProcessMemoryUsage(const String& sProcessName);
                DOTNETDUPE_API static DiskInfo GetProcessDiskUsage(const String& sProcessName);
                DOTNETDUPE_API static NetworkUsageInfo GetProcessNetworkUsage(const String& sProcessName);
                DOTNETDUPE_API static Collections::Generic::List<int> GetProcessNetworkPort(const String& sProcessName);
                DOTNETDUPE_API static ProcessNetworkConnectionInfo GetProcessNetworkInfo(const String& sProcessName);

                // Process-specific metrics by PID
                DOTNETDUPE_API static Collections::Generic::List<int> GetProcessNetworkPort(int iProcessId);
                DOTNETDUPE_API static ProcessNetworkConnectionInfo GetProcessNetworkInfo(int iProcessId);
                DOTNETDUPE_API static void EnrichProcessInfo(ProcessInfo& proc, bool bIncludeNetwork = true);

                // Progressive Streaming APIs
                DOTNETDUPE_API static SmartPointer<ProcessStreamer> CreateProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions());
                DOTNETDUPE_API static void EnumerateProcessesAsync(const Action<const ProcessInfo&>& fnOnProcess, const Action<>& fnOnComplete = nullptr);

                // Service enumeration API
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
