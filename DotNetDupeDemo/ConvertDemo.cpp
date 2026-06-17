#include "System/String.h"
#include "System/Convert.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateConvert() {
    Console::WriteLine("\n--- Convert Demonstration ---");

    // String to numeric
    String sIntStr("12345");
    int iIntVal = Convert::ToInt32(sIntStr);
    Console::Write("String '12345' to Int32: ");
    Console::WriteLine(iIntVal);

    // Numeric to String
    String sBoolStr = Convert::ToString(true);
    Console::Write("Bool true to String: ");
    Console::WriteLine(sBoolStr);

    // Base conversions
    String sHexStr = Convert::ToString(255, 16);
    Console::Write("Int 255 to Hex: ");
    Console::WriteLine(sHexStr);
}
