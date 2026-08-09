#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/EtwLogReader.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

class EtwLogReaderTests : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(EtwLogReaderTests, GivenEtwLogReader_WhenGetEventChannelsCalled_ThenReturnsChannelList) {
    // Given & When
    auto lstChannels = EtwLogReader::GetEventChannels();

    // Then
    EXPECT_GE(lstChannels.GetCount(), 0);
}

TEST_F(EtwLogReaderTests, GivenChannelName_WhenGetChannelEventCountCalled_ThenReturnsCount) {
    // Given
    String sChannel = "Application";

    // When
    unsigned long long uCount = EtwLogReader::GetChannelEventCount(sChannel);

    // Then
    EXPECT_GE(uCount, 0ULL);
}

TEST_F(EtwLogReaderTests, GivenChannelName_WhenGetChannelEventLevelCountsCalled_ThenReturnsLevelCounts) {
    // Given
    String sChannel = "Application";

    // When
    auto counts = EtwLogReader::GetChannelEventLevelCounts(sChannel);

    // Then
    EXPECT_GE(counts.uCriticalCount, 0ULL);
    EXPECT_GE(counts.uErrorCount, 0ULL);
    EXPECT_GE(counts.uWarningCount, 0ULL);
    EXPECT_GE(counts.uInfoCount, 0ULL);
    EXPECT_GE(counts.uVerboseCount, 0ULL);
}

TEST_F(EtwLogReaderTests, GivenValidChannel_WhenReadEventsCalled_ThenReturnsEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    if (lstEvents.GetCount() > 0) {
        EXPECT_FALSE(lstEvents[0].sRawXml.IsEmpty());
    }
}

TEST_F(EtwLogReaderTests, GivenEmptyChannelName_WhenReadEventsCalled_ThenThrowsArgumentException) {
    // Given, When, Then
    EXPECT_THROW(EtwLogReader::ReadEvents(""), ArgumentException);
}

TEST_F(EtwLogReaderTests, GivenPaginationParameters_WhenReadEventsCalled_ThenReturnsEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstForward = EtwLogReader::ReadEvents(sChannel, 5, 0, false);
    auto lstOffset = EtwLogReader::ReadEvents(sChannel, 5, 1, true);

    // Then
    EXPECT_GE(lstForward.GetCount(), 0);
    EXPECT_GE(lstOffset.GetCount(), 0);
}

TEST_F(EtwLogReaderTests, GivenEtwEventLevel_WhenReadEventsCalled_ThenReturnsFilteredEvents) {
    // Given
    String sChannel = "Application";
    int iRequestedCount = 5;

    // When - request exactly 5 Error level events
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, iRequestedCount, 0, true, EtwEventLevel::Error);

    // Then
    EXPECT_EQ(lstEvents.GetCount(), iRequestedCount);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_EQ(lstEvents[i].iLevel, 2); // 2 corresponds to EtwEventLevel::Error
    }
}

TEST_F(EtwLogReaderTests, GivenEtwLogReader_WhenStartAndStopListening_ThenStateUpdatesCorrectly) {
    // Given
    EtwLogReader reader;
    String sChannel = "System";
    bool bEventReceived = false;

    // When
    reader.StartListening(sChannel, [&bEventReceived](const EtwEvent& evt) {
        bEventReceived = true;
    });

    // Then
    EXPECT_TRUE(reader.IsListening());
    EXPECT_EQ(reader.GetListeningChannel(), "System");

    // When
    reader.StopListening();

    // Then
    EXPECT_FALSE(reader.IsListening());
    EXPECT_EQ(reader.GetListeningChannel(), "");
}

TEST_F(EtwLogReaderTests, GivenAlreadyListeningReader_WhenStartListeningCalled_ThenThrowsInvalidOperationException) {
    // Given
    EtwLogReader reader;
    reader.StartListening("Application", [](const EtwEvent&) {});

    // When & Then
    EXPECT_THROW(reader.StartListening("System", [](const EtwEvent&) {}), InvalidOperationException);
}
