#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Threading/Thread.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/InvalidOperationException.h"
#include <atomic>
#include <vector>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Threading;

class MockProcessObserver : public IProcessObserver {
public:
    std::atomic<int> m_iDiscoveredCount{0};
    std::atomic<int> m_iBatchCount{0};
    std::atomic<int> m_iUpdatedCount{0};
    std::atomic<bool> m_bCompleted{false};
    std::atomic<bool> m_bError{false};
    AutoResetEvent m_evtDone{false};

    void OnProcessDiscovered(const ProcessInfo& proc) override {
        (void)proc;
        m_iDiscoveredCount++;
    }

    void OnBatchReady(const Collections::Generic::List<ProcessInfo>& lstBatch) override {
        (void)lstBatch;
        m_iBatchCount++;
    }

    void OnProcessUpdated(const ProcessInfo& proc) override {
        (void)proc;
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
        if (proc.iProcessId > 0) iCount++;
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
        iTotalInBatches += lstBatch.GetCount();
    });

    pStreamer->OnCompleted([&evtDone]() {
        evtDone.Set();
    });

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
    std::atomic<bool> bDone{false};
    AutoResetEvent evtDone(false);

    SystemMetrics::EnumerateProcessesAsync(
        [&iProcCount](const ProcessInfo& proc) {
            if (proc.iProcessId > 0) iProcCount++;
        },
        [&bDone, &evtDone]() {
            bDone = true;
            evtDone.Set();
        });

    EXPECT_TRUE(evtDone.WaitOne(5000));
    EXPECT_TRUE(bDone.load());
    EXPECT_GT(iProcCount.load(), 0);
}
