#include "System/Guid.h"
#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateGuid() {
    Console::WriteLine("\n--- Guid Demonstration ---");
    Console::Write("New Guid: ");
    Console::WriteLine(Guid::NewGuid().ToString());
}
