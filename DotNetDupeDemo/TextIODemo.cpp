#include "System/String.h"

#include "System/Console.h"
#include "System/IO/StringReader.h"
#include "System/IO/StringWriter.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

#include "Demos.h"

void DemonstrateTextIO() {
    Console::WriteLine("\n--- Text I/O Demonstration ---");

    // StringWriter
    StringWriter pWriter;
    pWriter.WriteLine("First line");
    pWriter.WriteLine("Second line");
    pWriter.Write("Part of third line... ");
    pWriter.Write("rest of third line.");

    String sResult = pWriter.ToString();
    Console::WriteLine("Written content:");
    Console::WriteLine(sResult);

    // StringReader
    StringReader pReader(sResult);
    Console::WriteLine("Reading back line by line:");
    String sLine;
    int iLineNum = 1;
    while (!(sLine = pReader.ReadLine()).IsEmpty()) {
        Console::Write("Line ");
        Console::Write(iLineNum++);
        Console::Write(": ");
        Console::WriteLine(sLine);
    }
}
