#include "System/Console.h"
#include "System/Diagnostics/EtwLogReader.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateEtwLogReader() {
    Console::WriteLine("\n--- ETW Log Reader Demonstration ---");

    // 1. Enumerate Channels
    Console::WriteLine("Enumerating available Event Channels / Syslog categories:");
    auto lstChannels = EtwLogReader::GetEventChannels();
    int iDisplayCount = lstChannels.GetCount();
    if (iDisplayCount > 5) {
        iDisplayCount = 5;
    }
    for (int iIdx = 0; iIdx < iDisplayCount; iIdx++) {
        Console::WriteLine(" - Channel: " + lstChannels[iIdx]);
    }

    String sTargetChannel = "Application";
    if (lstChannels.GetCount() > 0) {
        sTargetChannel = lstChannels[0];
    }

    Console::WriteLine("\nDebugging Event Level Counts:");
    EtwLogReader::GetChannelEventLevelCounts("System");
    EtwLogReader::GetChannelEventLevelCounts("Setup");
    EtwLogReader::GetChannelEventLevelCounts("Application");

    Console::WriteLine("\nReading up to 3 'Error' events from channel: System");
    auto lstEvents = EtwLogReader::ReadEvents("System", 3, 0, true, EtwEventLevel::Error);
    for (int iIdx = 0; iIdx < lstEvents.GetCount(); iIdx++) {
        const auto& evt = lstEvents[iIdx];
        Console::Write(" [ID " + Convert::ToString(evt.iEventId) + "] ");
        Console::Write("Level: Error | ");
        Console::Write("Provider: " + evt.sProviderName + " | ");
        Console::Write("Message: " + evt.sMessage + "\n");
    }

    // 3. Live Subscription Listening
    EtwLogReader reader;
    Console::WriteLine("\nStarting live subscription listener on channel: " + sTargetChannel);
    reader.StartListening(sTargetChannel, [](const EtwEvent& evt) {
        Console::WriteLine(" [LIVE EVENT] ID: " + Convert::ToString(evt.iEventId) + " - " + evt.sMessage);
    });

    if (reader.IsListening()) {
        Console::WriteLine("Successfully attached live subscription to " + reader.GetListeningChannel());
    }

    reader.StopListening();
    Console::WriteLine("Stopped live event listener.");
}
