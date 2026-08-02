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
            };

        }
    }
}
