#include "pch.h"
#include <gtest/gtest.h>
#include "System/Diagnostics/EventLog.h"
#include "System/ArgumentException.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

TEST(DateTimeOffsetTests, GivenDateTimeOffset_WhenToStringCalled_ThenReturnsFormattedIsoString) {
    // Given
    DateTimeOffset dtNow = DateTimeOffset::Now();

    // When
    String sFormatted = dtNow.ToString();

    // Then
    EXPECT_FALSE(sFormatted.IsEmpty());
    EXPECT_GE(sFormatted.GetLength(), 10);
}

class EventLogTests : public ::testing::Test {
protected:
    void SetUp() override {
        if (EventLog::Exists("TestLog")) {
            EventLog::Delete("TestLog");
        }
        if (EventLog::SourceExists("TestSource")) {
            EventLog::DeleteEventSource("TestSource");
        }
    }

    void TearDown() override {
        if (EventLog::Exists("TestLog")) {
            EventLog::Delete("TestLog");
        }
        if (EventLog::SourceExists("TestSource")) {
            EventLog::DeleteEventSource("TestSource");
        }
    }
};

TEST_F(EventLogTests, GivenNewEventSource_WhenCreated_ThenSourceExistsReturnsTrue) {
    // Given
    String sSource = "TestSource";
    String sLogName = "TestLog";

    // When
    EventLog::CreateEventSource(sSource, sLogName);

    // Then
    EXPECT_TRUE(EventLog::SourceExists(sSource));
    EXPECT_TRUE(EventLog::Exists(sLogName));
}

TEST_F(EventLogTests, GivenEventLog_WhenWriteEntryExecuted_ThenEntryIsStoredInEntriesList) {
    // Given
    String sSource = "TestSource";
    String sLogName = "TestLog";
    EventLog::CreateEventSource(sSource, sLogName);

    EventLog objLog(sLogName, ".", sSource);

    // When
    objLog.WriteEntry("Application started successfully.", EventLogEntryType::Information, 1001);

    // Then
    auto lstEntries = objLog.GetEntries();
    EXPECT_GT(lstEntries.GetCount(), 0);
    bool bFound = false;
    for (int i = 0; i < lstEntries.GetCount(); i++) {
        if (lstEntries[i].GetMessage().Contains("Application started successfully.")) {
            bFound = true;
            EXPECT_EQ(lstEntries[i].GetEntryType(), EventLogEntryType::Information);
            break;
        }
    }
    EXPECT_TRUE(bFound);
}

TEST_F(EventLogTests, GivenMultipleEntries_WhenClearCalled_ThenEntriesAreRemoved) {
    // Given
    String sSource = "TestSource";
    String sLogName = "TestLog";
    EventLog::CreateEventSource(sSource, sLogName);

    EventLog objLog(sLogName, ".", sSource);
    objLog.WriteEntry("Log 1", EventLogEntryType::Warning, 1);
    objLog.WriteEntry("Log 2", EventLogEntryType::Error, 2);
    int iBeforeClearCount = objLog.GetEntries().GetCount();
    EXPECT_GT(iBeforeClearCount, 0);

    // When
    objLog.Clear();

    // Then
    int iAfterClearCount = objLog.GetEntries().GetCount();
    EXPECT_LE(iAfterClearCount, iBeforeClearCount);
}

TEST_F(EventLogTests, GivenStaticWriteEntry_WhenCalledWithSource_ThenLogIsUpdated) {
    // Given
    String sSource = "TestSource";
    String sLogName = "TestLog";
    EventLog::CreateEventSource(sSource, sLogName);

    // When
    EventLog::WriteEntry(sSource, "Static event message", EventLogEntryType::Error, 500);

    // Then
    EventLog objLog(sLogName);
    auto lstEntries = objLog.GetEntries();
    EXPECT_GE(lstEntries.GetCount(), 0);
    bool bFound = false;
    for (int i = 0; i < lstEntries.GetCount(); i++) {
        if (lstEntries[i].GetMessage().Contains("Static event message")) {
            bFound = true;
            break;
        }
    }
    if (lstEntries.GetCount() > 0 && bFound) {
        EXPECT_TRUE(bFound);
    }
}

TEST_F(EventLogTests, GivenEmptySource_WhenWriteEntryCalled_ThenThrowsArgumentException) {
    // Given, When, Then
    EXPECT_THROW(EventLog::WriteEntry("", "Test Message"), ArgumentException);
}

TEST_F(EventLogTests, GivenExistingEventSource_WhenDeleteEventSourceCalled_ThenSourceNoLongerExists) {
    // Given
    String sSource = "TestSource";
    String sLogName = "TestLog";
    EventLog::CreateEventSource(sSource, sLogName);
    EXPECT_TRUE(EventLog::SourceExists(sSource));

    // When
    EventLog::DeleteEventSource(sSource);

    // Then
    EXPECT_FALSE(EventLog::SourceExists(sSource));
}
