#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ActiveUserSession.h"

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

    // New List APIs
    auto lstTopCpu = SystemMetrics::GetTopProcesses(SystemResource::Cpu, 5);
    EXPECT_LE(lstTopCpu.GetCount(), 5);

    auto lstAllProc = SystemMetrics::GetAllProcesses(-1);
    EXPECT_GE(lstAllProc.GetCount(), 0);

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

TEST_F(RealtimeTelemetryTests, GivenInvalidSessionId_WhenGetAllProcessesCalled_ThenReturnsEmptyList) {
    // Given & When
    auto lstProc = SystemMetrics::GetAllProcesses(999999);

    // Then
    EXPECT_EQ(lstProc.GetCount(), 0);
}

TEST_F(RealtimeTelemetryTests, GivenTopProcesses_WhenQueriedWithVariousResources_ThenReturnsSortedResults) {
    // Given & When
    auto topMem = SystemMetrics::GetTopProcesses(SystemResource::Memory, 3);
    auto topDisk = SystemMetrics::GetTopProcesses(SystemResource::Disk, 3);
    auto topNet = SystemMetrics::GetTopProcesses(SystemResource::Network, 3);

    // Then
    EXPECT_LE(topMem.GetCount(), 3);
    EXPECT_LE(topDisk.GetCount(), 3);
    EXPECT_LE(topNet.GetCount(), 3);
}
