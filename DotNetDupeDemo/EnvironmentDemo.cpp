#include "System/Environment.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateEnvironment() {
    Console::WriteLine("\n--- Environment Demonstration ---");
    Console::Write("OS: ");
    Console::WriteLine(Environment::GetOperatingSystem().GetVersionString());
    Console::Write("Processor Count: ");
    Console::WriteLine((int)Environment::GetProcessorCount());
}
