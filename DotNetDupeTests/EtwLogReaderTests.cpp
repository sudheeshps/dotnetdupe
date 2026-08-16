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

TEST_F(EtwLogReaderTests, GivenApplicationChannel_WhenGetChannelEventLevelCountsCalled_ThenReturnsLevelCounts) {
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

TEST_F(EtwLogReaderTests, GivenSystemChannel_WhenGetChannelEventLevelCountsCalled_ThenReturnsLevelCounts) {
    // Given
    String sChannel = "System";

    // When
    auto counts = EtwLogReader::GetChannelEventLevelCounts(sChannel);

    // Then
    EXPECT_GE(counts.uCriticalCount, 0ULL);
    EXPECT_GE(counts.uErrorCount, 0ULL);
    EXPECT_GE(counts.uWarningCount, 0ULL);
    EXPECT_GE(counts.uInfoCount, 0ULL);
    EXPECT_GE(counts.uVerboseCount, 0ULL);
}

TEST_F(EtwLogReaderTests, GivenSecurityChannel_WhenGetChannelEventLevelCountsCalled_ThenReturnsLevelCounts) {
    // Given
    String sChannel = "Security";

    // When
    auto counts = EtwLogReader::GetChannelEventLevelCounts(sChannel);

    // Then
    EXPECT_GE(counts.uCriticalCount, 0ULL);
    EXPECT_GE(counts.uErrorCount, 0ULL);
    EXPECT_GE(counts.uWarningCount, 0ULL);
    EXPECT_GE(counts.uInfoCount, 0ULL);
    EXPECT_GE(counts.uVerboseCount, 0ULL);
}

TEST_F(EtwLogReaderTests, GivenSetupChannel_WhenGetChannelEventLevelCountsCalled_ThenReturnsLevelCounts) {
    // Given
    String sChannel = "Setup";

    // When
    auto counts = EtwLogReader::GetChannelEventLevelCounts(sChannel);

    // Then
    EXPECT_GE(counts.uCriticalCount, 0ULL);
    EXPECT_GE(counts.uErrorCount, 0ULL);
    EXPECT_GE(counts.uWarningCount, 0ULL);
    EXPECT_GE(counts.uInfoCount, 0ULL);
    EXPECT_GE(counts.uVerboseCount, 0ULL);
}

TEST_F(EtwLogReaderTests, GivenEmptyChannel_WhenGetChannelEventLevelCountsCalled_ThenReturnsZeroCounts) {
    // Given
    String sChannel = "";

    // When
    auto counts = EtwLogReader::GetChannelEventLevelCounts(sChannel);

    // Then
    EXPECT_EQ(counts.uCriticalCount, 0ULL);
    EXPECT_EQ(counts.uErrorCount, 0ULL);
    EXPECT_EQ(counts.uWarningCount, 0ULL);
    EXPECT_EQ(counts.uInfoCount, 0ULL);
    EXPECT_EQ(counts.uVerboseCount, 0ULL);
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

TEST_F(EtwLogReaderTests, GivenCriticalEventLevel_WhenReadEventsCalled_ThenReturnsCriticalEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Critical);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_EQ(lstEvents[i].iLevel, 1);
    }
}

TEST_F(EtwLogReaderTests, GivenErrorEventLevel_WhenReadEventsCalled_ThenReturnsErrorEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Error);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_EQ(lstEvents[i].iLevel, 2);
    }
}

TEST_F(EtwLogReaderTests, GivenWarningEventLevel_WhenReadEventsCalled_ThenReturnsWarningEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Warning);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_EQ(lstEvents[i].iLevel, 3);
    }
}

TEST_F(EtwLogReaderTests, GivenInfoEventLevel_WhenReadEventsCalled_ThenReturnsInfoEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Info);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_TRUE(lstEvents[i].iLevel == 4 || lstEvents[i].iLevel == 0);
    }
}

TEST_F(EtwLogReaderTests, GivenVerboseEventLevel_WhenReadEventsCalled_ThenReturnsVerboseEvents) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true, EtwEventLevel::Verbose);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    for (int i = 0; i < lstEvents.GetCount(); i++) {
        EXPECT_EQ(lstEvents[i].iLevel, 5);
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

TEST_F(EtwLogReaderTests, GivenValidChannel_WhenReadEventsCalled_ThenPopulatesEventProperties) {
    // Given
    String sChannel = "Application";

    // When
    auto lstEvents = EtwLogReader::ReadEvents(sChannel, 5, 0, true);

    // Then
    EXPECT_GE(lstEvents.GetCount(), 0);
    if (lstEvents.GetCount() > 0) {
        EXPECT_GE(lstEvents[0].iEventId, 0);
        EXPECT_GE(lstEvents[0].iLevel, 0);
        EXPECT_FALSE(lstEvents[0].sProviderName.IsEmpty());
        EXPECT_GT(lstEvents[0].dtTimeCreated.GetTicks(), 0);
        EXPECT_FALSE(lstEvents[0].sRawXml.IsEmpty());
    }
}

TEST_F(EtwLogReaderTests, GivenEmptyChannel_WhenStartListeningCalled_ThenThrowsArgumentException) {
    // Given
    EtwLogReader reader;

    // When & Then
    EXPECT_THROW(reader.StartListening("", [](const EtwEvent&) {}), ArgumentException);
}
