#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/TerminalSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

class TerminalSessionTests : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TerminalSessionTests, GivenTerminalServer_WhenGetSessionsCalled_ThenReturnsList) {
    // Given & When
    auto lstSessions = TerminalSession::GetSessions();

    // Then
    EXPECT_GE(lstSessions.GetCount(), 0);
}

TEST_F(TerminalSessionTests, GivenTerminalServer_WhenGetActiveSessionsCalled_ThenReturnsActiveSessions) {
    // Given & When
    auto lstActive = TerminalSession::GetActiveSessions();

    // Then
    for (int i = 0; i < lstActive.GetCount(); i++) {
        EXPECT_EQ(lstActive[i].eState, RdpSessionState::Active);
    }
}

TEST_F(TerminalSessionTests, GivenTerminalServer_WhenGetDisconnectedSessionsCalled_ThenReturnsDisconnectedSessions) {
    // Given & When
    auto lstDisconnected = TerminalSession::GetDisconnectedSessions();

    // Then
    for (int i = 0; i < lstDisconnected.GetCount(); i++) {
        EXPECT_EQ(lstDisconnected[i].eState, RdpSessionState::Disconnected);
    }
}
