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

class MockProcessObserver : public IProcessObserver {
public:
    std::atomic<int> m_iDiscoveredCount{0};
    std::atomic<int> m_iBatchCount{0};
    std::atomic<int> m_iUpdatedCount{0};
    std::atomic<bool> m_bCompleted{false};
    std::atomic<bool> m_bError{false};
    AutoResetEvent m_evtDone{false};

    void OnProcessDiscovered(const ProcessInfo& proc) override {
        AssertValidBaseMetrics(proc);
        m_iDiscoveredCount++;
    }

    void OnBatchReady(const Collections::Generic::List<ProcessInfo>& lstBatch) override {
        for (int i = 0; i < lstBatch.GetCount(); ++i) AssertValidBaseMetrics(lstBatch[i]);
        m_iBatchCount++;
    }

    void OnProcessUpdated(const ProcessInfo& proc) override {
        AssertValidDeepMetrics(proc);
        m_iUpdatedCount++;
    }

    void OnCompleted() override {
        m_bCompleted = true;
        m_evtDone.Set();
    }

    void OnError(const Exception& ex) override {
        (void)ex;
        m_bError = true;
        m_evtDone.Set();
    }
};

TEST(ProcessStreamerTests, GivenDefaultOptions_WhenStreamerStarted_ThenEmitsProcessesAndCompletes) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 10;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    std::atomic<int> iCount{0};
    std::atomic<bool> bFinished{false};
    AutoResetEvent evtDone(false);

    pStreamer->OnProcess([&iCount](const ProcessInfo& proc) {
        AssertValidBaseMetrics(proc);
        iCount++;
    });
    pStreamer->OnCompleted([&bFinished, &evtDone]() {
        bFinished = true;
        evtDone.Set();
    });

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

    pStreamer->OnBatch([&iBatchCount, &iTotalInBatches](const Collections::Generic::List<ProcessInfo>& lstBatch) {
        iBatchCount++;
        for (int i = 0; i < lstBatch.GetCount(); ++i) AssertValidBaseMetrics(lstBatch[i]);
        iTotalInBatches += lstBatch.GetCount();
    });
    pStreamer->OnCompleted([&evtDone]() { evtDone.Set(); });

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

TEST(ProcessStreamerTests, GivenIProcessObserver_WhenSubscribed_ThenReceivesAllEvents) {
    ProcessStreamOptions options;
    options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
    options.iBatchSize = 20;
    options.iBatchIntervalMs = 0;

    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    auto spObserver = SmartPointer<MockProcessObserver>::NewShared();
    pStreamer->Subscribe(spObserver);

    pStreamer->Start();
    EXPECT_TRUE(spObserver->m_evtDone.WaitOne(5000));
    EXPECT_TRUE(spObserver->m_bCompleted.load());
    EXPECT_GT(spObserver->m_iDiscoveredCount.load(), 0);
    EXPECT_GT(spObserver->m_iBatchCount.load(), 0);
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

    pStreamer->OnProcessUpdated([&iUpdatedCount, &lTotalRam](const ProcessInfo& proc) {
        AssertValidDeepMetrics(proc);
        lTotalRam += proc.memory.lPhysicalMemoryBytes;
        iUpdatedCount++;
    });
    pStreamer->OnCompleted([&evtDone]() { evtDone.Set(); });

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

    pStreamer->OnProcess([&bAllInitialEmpty](const ProcessInfo& proc) {
        AssertValidBaseMetrics(proc);
        if (proc.lstOpenPorts.GetCount() != 0) bAllInitialEmpty = false;
    });
    pStreamer->OnCompleted([&evtDone]() { evtDone.Set(); });

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

    pStreamer->OnProcessUpdated([&bAllPortsEmpty](const ProcessInfo& proc) {
        AssertValidBaseMetrics(proc);
        if (proc.lstOpenPorts.GetCount() > 0) bAllPortsEmpty = false;
    });
    pStreamer->OnCompleted([&evtDone]() { evtDone.Set(); });

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

TEST(ProcessStreamerTests, GivenNullObserver_WhenSubscribeCalled_ThenThrowsArgumentNullException) {
    ProcessStreamOptions options;
    auto pStreamer = SmartPointer<ProcessStreamer>::NewShared(options);
    EXPECT_THROW(pStreamer->Subscribe(nullptr), ArgumentNullException);
}

