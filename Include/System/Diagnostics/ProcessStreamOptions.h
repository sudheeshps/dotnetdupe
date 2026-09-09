#pragma once
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            /// \enum ProcessMetricsDetail
            /// \brief Granularity level for streaming process metrics discovery.
            enum class ProcessMetricsDetail {
                FastDiscoveryOnly = 0,  ///< Minimal discovery retrieving PID, name, and paths without deep inspection.
                Progressive = 1,        ///< Background progressive enrichment of CPU, memory, and disk usage.
                FullSynchronized = 2    ///< Full synchronous inspection including open ports and network sockets.
            };

            /// \struct ProcessStreamOptions
            /// \brief Configuration options controlling the execution of a ProcessStreamer.
            struct ProcessStreamOptions {
                ProcessMetricsDetail eDetailLevel;  ///< Depth of telemetry gathered for each process.
                int iBatchSize;                     ///< Number of processes dispatched in each batch event.
                int iBatchIntervalMs;               ///< Millisecond interval between batch dispatches.
                int iSessionId;                     ///< Filter by session identifier (-1 for all sessions).
                bool bIncludeNetworkInfo;           ///< True to enumerate open ports and socket connections.

                /// \brief Default constructor configuring progressive telemetry defaults.
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
