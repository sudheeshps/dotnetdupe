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

TEST_F(RealtimeTelemetryTests, GivenSystemMetrics_WhenGetSystemMetricsCalled_ThenReturnsValidMetrics) {
    // Given & When
    RealTimeSystemInfo data = SystemMetrics::GetSystemMetrics();

    // Then
    EXPECT_GE(data.dCpuUsagePercent, 0.0);
    EXPECT_GE(data.dMemoryUsagePercent, 0.0);
    EXPECT_GE(data.uMemoryTotalBytes, 0ULL);
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
