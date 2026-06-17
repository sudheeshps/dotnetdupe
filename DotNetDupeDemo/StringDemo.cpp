#include "System/Array.h"
#include "System/String.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateString() {
    Console::WriteLine("\n--- String Demonstration ---");
    
    String sS1 = "   Hello World   ";
    Console::Write("Original: '");
    Console::Write(sS1);
    Console::WriteLine("'");
    
    Console::Write("Trimmed: '");
    Console::Write(sS1.Trim());
    Console::WriteLine("'");
    
    Console::Write("Lower: ");
    Console::WriteLine(sS1.ToLower());
    
    Console::Write("Upper: ");
    Console::WriteLine(sS1.ToUpper());

    String sS2 = "Apple,Banana,Cherry";
    Array<String> arrParts = sS2.Split(',');
    Console::WriteLine("Splitting 'Apple,Banana,Cherry':");
    for(const auto& sPart : arrParts) {
        Console::Write("  - ");
        Console::WriteLine(sPart);
    }

    // String::Format Demonstration
    Console::WriteLine("\nString::Format Demonstration:");
    String sName = "Alice";
    int iAge = 30;
    double dScore = 95.5;
    bool bActive = true;
    
    String sFormatted = String::Format("User Profile: Name={0}, Age={1}, Score={2}, Active={3}", sName, iAge, dScore, bActive);
    Console::WriteLine(sFormatted);

    // Escaped Braces
    String sEscaped = String::Format("Position placeholder format uses {{0}}, formatted value is {0}", 42);
    Console::WriteLine(sEscaped);
}
