#include "System/String.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateConsole() {
    Console::WriteLine("\n--- Console Demonstration ---");
    Console::Write("MOCK INPUT: Enter your name: ");
    Console::SetIn("Gemini User"); 
    String sName = Console::ReadLine();
    Console::Write("Hello, ");
    Console::WriteLine(sName);
    
    // Clear mock input so subsequent ReadLine calls wait for actual stdin
    Console::ClearInputs(); 

    // Demonstrate parameter interpolation
    Console::WriteLine("Interpolated Output: Hello {0}, welcome to {1}!", sName, "DotNetDupe");

    Console::WriteLine("Available Colors:");
    Console::SetForegroundColor(ConsoleColor::Green);
    Console::WriteLine("  This is Green text");
    Console::SetForegroundColor(ConsoleColor::Cyan);
    Console::WriteLine("  This is Cyan text");
    Console::ResetColor();
    
    Console::SetTitle("DotNetDupe Demo Application");
}
