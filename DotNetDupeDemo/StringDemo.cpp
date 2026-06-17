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
}
