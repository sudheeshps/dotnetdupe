#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Threading/Thread.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/InvalidOperationException.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include <atomic>
#include <vector>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

static void AssertValidBaseMetrics(const ProcessInfo& proc) {
    EXPECT_GT(proc.iProcessId, 0);
    EXPECT_FALSE(proc.sName.IsEmpty());
    EXPECT_GE(proc.memory.lPhysicalMemoryBytes, 0LL);
    EXPECT_GE(proc.dCpuUsagePercent, 0.0);
    EXPECT_LE(proc.dCpuUsagePercent, 100.0);
}

static void AssertValidPortsAndConnections(const ProcessInfo& proc) {
    EXPECT_GE(proc.lstOpenPorts.GetCount(), 0);
    for (int i = 0; i < proc.lstOpenPorts.GetCount(); ++i) {
        EXPECT_GT(proc.lstOpenPorts[i], 0);
        EXPECT_LE(proc.lstOpenPorts[i], 65535);
    }
    EXPECT_GE(proc.lstConnections.GetCount(), 0);
    for (int i = 0; i < proc.lstConnections.GetCount(); ++i) {
        EXPECT_GT(proc.lstConnections[i].iLocalPort, 0);
        EXPECT_FALSE(proc.lstConnections[i].sState.IsEmpty());
    }
}

static void AssertValidDeepMetrics(const ProcessInfo& proc) {
    AssertValidBaseMetrics(proc);
    EXPECT_GE(proc.disk.lDiskReadBytes, -1LL);
    EXPECT_GE(proc.disk.lDiskWriteBytes, -1LL);
    EXPECT_GE(proc.network.lNetworkReadBytes, -1LL);
    EXPECT_GE(proc.network.lNetworkWriteBytes, -1LL);
    AssertValidPortsAndConnections(proc);
}

TEST(ProcessStreamerTests, GivenDefaultOptions_WhenStreamerStarted_ThenEmitsProcessesAndCompletes) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<int> iCount{0};
    std::atomic<bool> bFinished{false};
    AutoResetEvent evtDone(false);

    pStreamer->ProcessDiscovered += [&iCount](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        AssertValidBaseMetrics(e.GetProcess());
        iCount++;
    };
    pStreamer->Completed += [&bFinished, &evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        bFinished = true;
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_TRUE(bFinished.load());
    EXPECT_GT(iCount.load(), 0);
    EXPECT_FALSE(pStreamer->IsRunning());
}

TEST(ProcessStreamerTests, GivenCustomBatchSize_WhenStreamerStarted_ThenEmitsBatchesInConfiguredSizes) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 15;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<int> iBatchCount{0};
    std::atomic<int> iTotalInBatches{0};
    AutoResetEvent evtDone(false);

    pStreamer->BatchReady += [&iBatchCount, &iTotalInBatches](const void* pSender, const ProcessBatchEventArgs& e) {
        (void)pSender;
        iBatchCount++;
        for (int i = 0; i < e.GetBatch().GetCount(); ++i) AssertValidBaseMetrics(e.GetBatch()[i]);
        iTotalInBatches += e.GetBatch().GetCount();
    };
    pStreamer->Completed += [&evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_GT(iBatchCount.load(), 0);
    EXPECT_GT(iTotalInBatches.load(), 0);
}

TEST(ProcessStreamerTests, GivenRunningStreamer_WhenCancelled_ThenStopsGracefully) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::Progressive;
    options.iBatchSize = 5;
    options.iBatchIntervalMs = 100;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    pStreamer->Start();
    EXPECT_TRUE(pStreamer->IsRunning());

    Thread::Sleep(50);
    pStreamer->Cancel();
    EXPECT_FALSE(pStreamer->IsRunning());
}

TEST(ProcessStreamerTests, GivenMulticastSubscribers_WhenProcessesDiscovered_ThenAllSubscribersReceiveEvents) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 20;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<int> iCount1{0};
    std::atomic<int> iCount2{0};
    AutoResetEvent evtDone(false);

    size_t nToken1 = (pStreamer->ProcessDiscovered += [&iCount1](const void*, const ProcessEventArgs& e) {
        AssertValidBaseMetrics(e.GetProcess());
        iCount1++;
    });

    size_t nToken2 = (pStreamer->ProcessDiscovered += [&iCount2](const void*, const ProcessEventArgs& e) {
        AssertValidBaseMetrics(e.GetProcess());
        iCount2++;
    });

    pStreamer->Completed += [&evtDone](const void*, const EventArgs&) {
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_GT(iCount1.load(), 0);
    EXPECT_EQ(iCount1.load(), iCount2.load());

    // Verify unsubscription
    EXPECT_TRUE(pStreamer->ProcessDiscovered -= nToken1);
    EXPECT_TRUE(pStreamer->ProcessDiscovered -= nToken2);
}

TEST(ProcessStreamerTests, GivenRunningStreamer_WhenStartCalledAgain_ThenThrowsInvalidOperationException) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::Progressive;
    options.iBatchSize = 5;
    options.iBatchIntervalMs = 200;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    pStreamer->Start();
    EXPECT_THROW(pStreamer->Start(), InvalidOperationException);

    pStreamer->Cancel();
}

TEST(ProcessStreamerTests, GivenSystemMetricsStaticHelper_WhenAsyncEnumerated_ThenProcessesEmitted) {
    std::atomic<int> iProcCount{0};
    AutoResetEvent evtDone(false);

    SystemMetrics::EnumerateProcessesAsync(
        [&iProcCount](const ProcessInfo& proc) {
            AssertValidBaseMetrics(proc);
            iProcCount++;
        },
        [&evtDone]() { evtDone.Set(); });

    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_GT(iProcCount.load(), 0);
}

TEST(ProcessStreamerTests, GivenProgressiveStreamer_WhenDeepEnrichmentRuns_ThenEmitsUpdatedProcessWithNetworkTelemetry) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::Progressive;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;
    options.bIncludeNetworkInfo = true;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<int> iUpdatedCount{0};
    std::atomic<long long> lTotalRam{0};
    AutoResetEvent evtDone(false);

    pStreamer->ProcessUpdated += [&iUpdatedCount, &lTotalRam](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        AssertValidDeepMetrics(e.GetProcess());
        lTotalRam += e.GetProcess().memory.lPhysicalMemoryBytes;
        iUpdatedCount++;
    };
    pStreamer->Completed += [&evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne());
    EXPECT_GT(iUpdatedCount.load(), 0);
    EXPECT_GT(lTotalRam.load(), 0LL);
}

TEST(ProcessStreamerTests, GivenFastDiscoveryOnly_WhenStreamerRuns_ThenOpenPortsRemainEmptyInitially) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<bool> bAllInitialEmpty{true};
    AutoResetEvent evtDone(false);

    pStreamer->ProcessDiscovered += [&bAllInitialEmpty](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        AssertValidBaseMetrics(e.GetProcess());
        if (e.GetProcess().lstOpenPorts.GetCount() != 0) bAllInitialEmpty = false;
    };
    pStreamer->Completed += [&evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_TRUE(bAllInitialEmpty.load());
}

TEST(ProcessStreamerTests, GivenOptionsWithIncludeNetworkFalse_WhenProgressiveStreamerRuns_ThenNetworkInfoIsSkipped) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::Progressive;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;
    options.bIncludeNetworkInfo = false;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<bool> bAllPortsEmpty{true};
    AutoResetEvent evtDone(false);

    pStreamer->ProcessUpdated += [&bAllPortsEmpty](const void* pSender, const ProcessEventArgs& e) {
        (void)pSender;
        AssertValidBaseMetrics(e.GetProcess());
        if (e.GetProcess().lstOpenPorts.GetCount() > 0) bAllPortsEmpty = false;
    };
    pStreamer->Completed += [&evtDone](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        evtDone.Set();
    };

    pStreamer->Start();
    EXPECT_TRUE(evtDone.WaitOne());
    EXPECT_TRUE(bAllPortsEmpty.load());
}

TEST(ProcessStreamerTests, GivenNegativeBatchOptions_WhenStartCalled_ThenThrowsArgumentException) {
    ProcessStreamOptions options;
    options.iBatchSize = -5;
    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    EXPECT_THROW(pStreamer->Start(), ArgumentException);
}
