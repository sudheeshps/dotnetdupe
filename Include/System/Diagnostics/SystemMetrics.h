#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            struct ProcessResourceInfo {
                int iProcessId;
                String sName;
                String sPath;
                String sCommandLine;
                double dCpuUsagePercent;
                long long lMemoryUsageBytes;
                long long lDiskReadBytes;
                long long lDiskWriteBytes;
                long long lNetworkReadBytes;
                long long lNetworkWriteBytes;
                Collections::Generic::List<int> lstOpenPorts;
                bool bHasEstablishedInboundConnection;
            };

            struct RealTimeSystemInfo {
                double dCpuUsagePercent;
                double dMemoryUsagePercent;
                unsigned long long uMemoryTotalBytes;
                unsigned long long uMemoryUsedBytes;
                unsigned long long uDiskReadBytes;
                unsigned long long uDiskWriteBytes;
                double dNetworkUsageMbps;
                Collections::Generic::List<ProcessResourceInfo> lstTopProcesses;

                RealTimeSystemInfo()
                    : dCpuUsagePercent(0.0),
                      dMemoryUsagePercent(0.0),
                      uMemoryTotalBytes(0ULL),
                      uMemoryUsedBytes(0ULL),
                      uDiskReadBytes(0ULL),
                      uDiskWriteBytes(0ULL),
                      dNetworkUsageMbps(0.0) {}
            };

            class SystemMetrics : public Object {
            public:
                DOTNETDUPE_API SystemMetrics();
                DOTNETDUPE_API virtual ~SystemMetrics();

                DOTNETDUPE_API static RealTimeSystemInfo GetSystemMetrics();

            private:
#if defined(_WIN32)
                static void ReadWin32Memory(RealTimeSystemInfo& data);
                static void ReadWin32Cpu(RealTimeSystemInfo& data);
                static void ReadWin32Disk(RealTimeSystemInfo& data);
                static void ReadWin32Network(RealTimeSystemInfo& data);
                static void ReadWin32TopProcesses(RealTimeSystemInfo& data);
                static void PopulateProcessInfo(void* pEntry32, ProcessResourceInfo& proc);
                static void ReadProcessCommandLine(void* hProc, void* pPebBaseAddress, ProcessResourceInfo& proc, bool& bCmdRead);
                static void ReadProcessIoAndTimes(void* hProc, ProcessResourceInfo& proc);
                static void ReadProcessNetwork(void* hProc, ProcessResourceInfo& proc);
                static void ReadProcessPortsAndConnections(int iProcessId, ProcessResourceInfo& proc);
#endif
            };

        }
    }
}
