#include "System/String.h"

#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

#include "Demos.h"

void DemonstrateCollections() {
    Console::WriteLine("\n--- Collections Demonstration ---");

    // List
    List<int> lstNumbers = { 5, 2, 8, 1, 9 };
    lstNumbers.Add(4);
    lstNumbers.Sort();
    
    Console::Write("Sorted List: ");
    for (int iN : lstNumbers) {
        Console::Write(iN);
        Console::Write(" ");
    }
    Console::WriteLine();

    // List Functional
    Console::Write("Numbers > 5: ");
    List<int> lstBigNumbers = lstNumbers.FindAll([](int iN) { return iN > 5; });
    for (int iN : lstBigNumbers) {
        Console::Write(iN);
        Console::Write(" ");
    }
    Console::WriteLine();

    // Dictionary
    Dictionary<String, String> dictMeta;
    dictMeta.Add("OS", "Windows");
    dictMeta.Add("Arch", "x64");

    Console::WriteLine("Dictionary Meta:");
    for (auto const& [sKey, sVal] : dictMeta) {
        Console::Write("  ");
        Console::Write(sKey);
        Console::Write(" => ");
        Console::WriteLine(sVal);
    }
}
