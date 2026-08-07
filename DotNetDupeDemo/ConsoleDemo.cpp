#include "System/String.h"
#include "System/Console.h"
#include "System/IO/StringWriter.h"
#include "System/IO/StringReader.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>
#include <memory>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

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

    // Demonstrate stream redirection
    SmartPointer<StringWriter> pWriter(true);
    Console::SetOut(pWriter);
    Console::WriteLine("This string is captured via SetOut redirection!");
    Console::Clear(); // Reset redirection streams and output buffers

    Console::WriteLine("Available Colors:");
    Console::SetForegroundColor(ConsoleColor::Green);
    Console::WriteLine("  This is Green text");
    Console::SetForegroundColor(ConsoleColor::Cyan);
    Console::WriteLine("  This is Cyan text");
    Console::ResetColor();
    
    Console::SetTitle("DotNetDupe Demo Application");
}
