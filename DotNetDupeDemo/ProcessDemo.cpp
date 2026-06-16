#include "System/String.h"
#include "System/Diagnostics/Process.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

#include "Demos.h"

void DemonstrateProcess() {
    Console::WriteLine("\n--- Process Demonstration ---");

#if defined(_WIN32)
    String sFileName = "cmd.exe";
    String sArguments = "/c echo Hello from child process! && exit 42";
#else
    String sFileName = "/bin/sh";
    String sArguments = "-c \"echo Hello from child process!; exit 42\"";
#endif

    Console::Write("Starting process: ");
    Console::WriteLine(sFileName);

    auto pProcess = Process::Start(sFileName, sArguments);
    if (!pProcess.IsNull()) {
        Console::Write("Process started with ID: ");
        Console::WriteLine(pProcess->GetId());

        Console::WriteLine("Waiting for process to exit...");
        pProcess->WaitForExit();

        Console::Write("Process exited with code: ");
        Console::WriteLine(pProcess->GetExitCode());
    } else {
        Console::WriteLine("Failed to start process.");
    }
}
