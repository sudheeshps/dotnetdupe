#include "System/Console.h"
#include "System/Diagnostics/EventLog.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateEventLog() {
    Console::WriteLine("\n--- Cross-Platform EventLog Demonstration ---");

    String sSource = "DemoAppService";
    String sLogName = "Application";

    if (!EventLog::SourceExists(sSource)) {
        EventLog::CreateEventSource(sSource, sLogName);
        Console::WriteLine("Created event source '" + sSource + "' registered to '" + sLogName + "'.");
    }

    // 1. Instanced Write (Windows EventLog / Linux Syslog)
    EventLog objLog(sLogName, ".", sSource);
    objLog.WriteEntry("Service initialized successfully.", EventLogEntryType::Information, 100);
    objLog.WriteEntry("Low disk space warning on drive C:", EventLogEntryType::Warning, 201);
    objLog.WriteEntry("Failed to connect to primary database server.", EventLogEntryType::Error, 500);

    // 2. Static Write
    EventLog::WriteEntry(sSource, "Audit record written via static method.", EventLogEntryType::SuccessAudit, 300);

    // 3. Read Entries (Reads OS EventLog on Windows / In-memory store)
    Console::WriteLine("\nReading entries from EventLog '" + sLogName + "':");
    auto lstEntries = objLog.GetEntries();
    for (int iIdx = 0; iIdx < lstEntries.GetCount(); iIdx++) {
        const auto& entry = lstEntries[iIdx];
        Console::Write(" [ID " + Convert::ToString(entry.GetInstanceId()) + "] ");
        Console::Write("Source: " + entry.GetSource() + " | ");
        Console::Write("Message: " + entry.GetMessage() + "\n");
    }

    objLog.Clear();
    Console::WriteLine("EventLog entries cleared.");
}
