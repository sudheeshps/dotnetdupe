#pragma once
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            enum class ProcessMetricsDetail {
                FastDiscoveryOnly = 0,
                Progressive = 1,
                FullSynchronized = 2
            };

            struct ProcessStreamOptions {
                ProcessMetricsDetail eDetailLevel;
                int iBatchSize;
                int iBatchIntervalMs;
                int iSessionId;
                bool bIncludeNetworkInfo;

                ProcessStreamOptions()
                    : eDetailLevel(ProcessMetricsDetail::Progressive),
                      iBatchSize(25),
                      iBatchIntervalMs(50),
                      iSessionId(-1),
                      bIncludeNetworkInfo(true) {}
            };

        }
    }
}
