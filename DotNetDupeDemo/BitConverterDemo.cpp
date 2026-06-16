#include "System/Array.h"
#include "System/String.h"
#include "System/BitConverter.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateBitConverter() {
    Console::WriteLine("\n--- BitConverter Demonstration ---");

    int iIntVal = 123456789;
    Array<byte> arrIntBytes = BitConverter::GetBytes(iIntVal);
    Console::Write("Int: 123456789 to bytes: ");
    String sHexStr = BitConverter::ToString(arrIntBytes);
    Console::WriteLine(sHexStr);

    int iBackToInt = BitConverter::ToInt32(arrIntBytes, 0);
    Console::Write("Bytes back to int: ");
    Console::WriteLine(iBackToInt);
}
