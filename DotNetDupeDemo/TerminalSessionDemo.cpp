#include "System/Console.h"
#include "System/Diagnostics/TerminalSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

void DemonstrateTerminalSession() {
    Console::WriteLine("\n=== Terminal Session Demonstration ===");

    auto lstSessions = TerminalSession::GetSessions();
    Console::WriteLine("Total Sessions Enumerated: " + String::ValueOf(lstSessions.GetCount()));

    for (int i = 0; i < lstSessions.GetCount(); ++i) {
        const auto& session = lstSessions[i];
        Console::WriteLine("\nSession ID: " + String::ValueOf(static_cast<int>(session.uSessionId)));
        Console::WriteLine("  Name: " + session.sSessionName);
        Console::WriteLine("  User: " + session.sUserName);
        Console::WriteLine("  Domain: " + session.sDomainName);
        Console::WriteLine("  Client Name: " + session.sClientName);
        Console::WriteLine("  Is RDP: " + String(session.bIsRdpSession ? "True" : "False"));
    }
}
