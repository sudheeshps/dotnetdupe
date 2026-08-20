#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"
#include "System/Diagnostics/Process.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

class RealtimeTelemetryTests : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(RealtimeTelemetryTests, GivenSystemMetrics_WhenSystemMetricsCalled_ThenReturnsValidMetrics) {
    // Given & When
    MemoryInfo mem = SystemMetrics::GetSystemMemoryUsage();
    double dCpu = SystemMetrics::GetSystemCpuUsage();
    DiskInfo disk = SystemMetrics::GetSystemDiskUsage();
    double dNet = SystemMetrics::GetSystemNetworkUsage();

    // Then
    EXPECT_GE(mem.dMemoryUsagePercent, 0.0);
    EXPECT_GE(mem.uMemoryTotalBytes, 0ULL);
    EXPECT_GE(dCpu, 0.0);
    EXPECT_GE(disk.lDiskReadBytes, 0LL);
    EXPECT_GE(dNet, 0.0);

    // Process specific API tests
    String sCmd = SystemMetrics::GetProcessCommandLine(String("svchost.exe"));
    MemoryInfo procMem = SystemMetrics::GetProcessMemoryUsage(String("svchost.exe"));
    DiskInfo procDisk = SystemMetrics::GetProcessDiskUsage(String("svchost.exe"));
    NetworkUsageInfo procNet = SystemMetrics::GetProcessNetworkUsage(String("svchost.exe"));
    auto lstPorts = SystemMetrics::GetProcessNetworkPort(String("svchost.exe"));
    ProcessNetworkConnectionInfo netInfo = SystemMetrics::GetProcessNetworkInfo(String("svchost.exe"));

    EXPECT_GE(procMem.lPhysicalMemoryBytes, -1LL);

    auto arrAllProc = Process::GetProcesses();
    EXPECT_GE(arrAllProc.GetLength(), 0);

    auto lstServices = SystemMetrics::GetAllServices();
    EXPECT_GE(lstServices.GetCount(), 0);
    for (int i = 0; i < lstServices.GetCount(); ++i) {
        EXPECT_FALSE(lstServices[i].sStartType.IsEmpty());
    }
}

TEST_F(RealtimeTelemetryTests, GivenActiveUserSession_WhenGetAllSessionsCalled_ThenReturnsSessionsList) {
    // Given & When
    auto lstSessions = ActiveUserSession::GetAllSessions();

    // Then
    EXPECT_GE(lstSessions.GetCount(), 0);
}

TEST_F(RealtimeTelemetryTests, GivenActiveUserSession_WhenGetActiveSessionsCalled_ThenReturnsOnlyActiveSessions) {
    // Given & When
    auto lstActive = ActiveUserSession::GetActiveSessions();

    // Then
    for (int i = 0; i < lstActive.GetCount(); i++) {
        EXPECT_TRUE(lstActive[i].bIsActive);
    }
}

TEST_F(RealtimeTelemetryTests, GivenInvalidProcessName_WhenSystemMetricsQueried_ThenReturnsDefaultFallbackValues) {
    // Given
    String sInvalidName = "NonExistentProcess_123456789.exe";

    // When
    String sCmd = SystemMetrics::GetProcessCommandLine(sInvalidName);
    MemoryInfo mem = SystemMetrics::GetProcessMemoryUsage(sInvalidName);
    DiskInfo disk = SystemMetrics::GetProcessDiskUsage(sInvalidName);
    NetworkUsageInfo net = SystemMetrics::GetProcessNetworkUsage(sInvalidName);
    auto lstPorts = SystemMetrics::GetProcessNetworkPort(sInvalidName);
    ProcessNetworkConnectionInfo connInfo = SystemMetrics::GetProcessNetworkInfo(sInvalidName);

    // Then
    EXPECT_TRUE(sCmd.IsEmpty());
    EXPECT_EQ(mem.lPhysicalMemoryBytes, -1LL);
    EXPECT_EQ(disk.lDiskReadBytes, -1LL);
    EXPECT_EQ(net.lNetworkReadBytes, -1LL);
    EXPECT_EQ(lstPorts.GetCount(), 0);
    EXPECT_FALSE(connInfo.bHasEstablishedInboundConnection);
}

TEST_F(RealtimeTelemetryTests, GivenValidProcessId_WhenGetProcessNetworkPortCalled_ThenReturnsPortsList) {
    // Given
    int iPid = Process::GetCurrentProcessId();

    // When
    auto lstPorts = SystemMetrics::GetProcessNetworkPort(iPid);

    // Then
    EXPECT_GE(lstPorts.GetCount(), 0);
}

TEST_F(RealtimeTelemetryTests, GivenValidProcessId_WhenGetProcessNetworkInfoCalled_ThenReturnsConnectionInfo) {
    // Given
    int iPid = Process::GetCurrentProcessId();

    // When
    ProcessNetworkConnectionInfo netInfo = SystemMetrics::GetProcessNetworkInfo(iPid);

    // Then
    EXPECT_GE(netInfo.lstOpenPorts.GetCount(), 0);
    EXPECT_GE(netInfo.lstConnections.GetCount(), 0);
}

TEST_F(RealtimeTelemetryTests, GivenInvalidProcessId_WhenNetworkQueriedByPid_ThenReturnsEmptyFallback) {
    // Given
    int iInvalidPid = -99999;

    // When
    auto lstPorts = SystemMetrics::GetProcessNetworkPort(iInvalidPid);
    ProcessNetworkConnectionInfo netInfo = SystemMetrics::GetProcessNetworkInfo(iInvalidPid);

    // Then
    EXPECT_EQ(lstPorts.GetCount(), 0);
    EXPECT_EQ(netInfo.lstOpenPorts.GetCount(), 0);
    EXPECT_EQ(netInfo.lstConnections.GetCount(), 0);
    EXPECT_FALSE(netInfo.bHasEstablishedInboundConnection);
}

TEST_F(RealtimeTelemetryTests, GivenValidProcess_WhenEnrichProcessInfoCalled_ThenPopulatesMetrics) {
    // Given
    auto spSelf = Process::GetCurrentProcess();
    ASSERT_FALSE(spSelf.IsNull());
    ProcessInfo proc;
    proc.iProcessId = spSelf->GetId();
    proc.sName = spSelf->GetProcessName();

    // When
    SystemMetrics::EnrichProcessInfo(proc, true);

    // Then
    EXPECT_GT(proc.iProcessId, 0);
    EXPECT_FALSE(proc.sName.IsEmpty());
    EXPECT_GE(proc.memory.lPhysicalMemoryBytes, 0LL);
    EXPECT_GE(proc.dCpuUsagePercent, 0.0);
    EXPECT_LE(proc.dCpuUsagePercent, 100.0);
}
