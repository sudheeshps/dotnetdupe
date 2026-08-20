#include "System/String.h"
#include "System/Diagnostics/Process.h"
#include "System/Console.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

#include "Demos.h"

void DemonstrateProcess() {
    Console::WriteLine("\n--- Process Demonstration ---");

    // 1. Current Process Inspection
    auto spSelf = Process::GetCurrentProcess();
    Console::WriteLine("Current Process: " + spSelf->GetProcessName() + " (PID: " + Convert::ToString(spSelf->GetId()) + ")");

    // 2. Discover Running Processes
    auto arrProcesses = Process::GetProcesses();
    Console::WriteLine("Total Running Processes: " + Convert::ToString(arrProcesses.GetLength()));

    // 3. Find Processes by Name
    auto arrSvchost = Process::GetProcessesByName("svchost");
    Console::WriteLine("Running instances of svchost: " + Convert::ToString(arrSvchost.GetLength()));

    // 4. Start Child Process
#if defined(_WIN32)
    String sFileName = "cmd.exe";
    String sArguments = "/c echo Hello from child process! && exit 42";
#else
    String sFileName = "/bin/sh";
    String sArguments = "-c \"echo Hello from child process!; exit 42\"";
#endif

    Console::Write("Starting process: ");
    Console::WriteLine(sFileName);

    auto spProcess = Process::Start(sFileName, sArguments);
    if (!spProcess.IsNull()) {
        Console::Write("Process started with ID: ");
        Console::WriteLine(spProcess->GetId());

        Console::WriteLine("Waiting for process to exit...");
        spProcess->WaitForExit();

        Console::Write("Process exited with code: ");
        Console::WriteLine(spProcess->GetExitCode());
    } else {
        Console::WriteLine("Failed to start process.");
    }
}
