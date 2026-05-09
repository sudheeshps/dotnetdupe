#include "System/Array.h"
#include "System/String.h"
#include "System/IO/File.h"
#include "System/IO/FileStream.h"
#include "System/Environment.h"
#include "System/Buffer.h"
#include "System/Char.h"
#include "System/Path.h"
#include "System/Random.h"
#include "System/Uri.h"
#include "System/Version.h"
#include "System/OperatingSystem.h"
#include "System/BitConverter.h"
#include "System/Convert.h"
#include "System/TimeProvider.h"
#include "System/Diagnostics/Stopwatch.h"
#include "System/Text/StringBuilder.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Guid.h"
#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <thread>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Diagnostics;
using namespace DotNetDupe::System::Text;
using namespace DotNetDupe::System::Collections::Generic;

void DemonstrateConsole() {
    Console::WriteLine(_T("\n--- Console Demonstration ---"));
    Console::Write(_T("MOCK INPUT: Enter your name: "));
    Console::SetIn(_T("Gemini User")); 
    String name = Console::ReadLine();
    Console::Write(_T("Hello, "));
    Console::WriteLine(name);

    Console::WriteLine(_T("Available Colors:"));
    Console::SetForegroundColor(ConsoleColor::Green);
    Console::WriteLine(_T("  This is Green text"));
    Console::ResetColor();
}

void DemonstrateBitConverter() {
    Console::WriteLine(_T("\n--- BitConverter Demonstration ---"));

    int intVal = 123456789;
    Array<byte> intBytes = BitConverter::GetBytes(intVal);
    Console::Write(_T("Int: 123456789 to bytes: "));
    String hexStr = BitConverter::ToString(intBytes);
    Console::WriteLine(hexStr);

    int backToInt = BitConverter::ToInt32(intBytes, 0);
    Console::Write(_T("Bytes back to int: "));
    Console::WriteLine(backToInt);
}

void DemonstrateConvert() {
    Console::WriteLine(_T("\n--- Convert Demonstration ---"));

    // String to numeric
    String intStr(_T("12345"));
    int intVal = Convert::ToInt32(intStr);
    Console::Write(_T("String '12345' to Int32: "));
    Console::WriteLine(intVal);

    String doubleStr(_T("3.14159"));
    double doubleVal = Convert::ToDouble(doubleStr);
    Console::Write(_T("String '3.14159' to Double: "));
    Console::WriteLine(doubleVal);

    // Numeric to String
    String boolStr = Convert::ToString(true);
    Console::Write(_T("Bool true to String: "));
    Console::WriteLine(boolStr);

    // Base conversions
    String hexStr = Convert::ToString(255, 16);
    Console::Write(_T("Int 255 to Hex: "));
    Console::WriteLine(hexStr);

    int fromHex = Convert::ToInt32(String(_T("FF")), 16);
    Console::Write(_T("Hex 'FF' to Int32: "));
    Console::WriteLine(fromHex);
}

void DemonstrateTimeProvider() {
    Console::WriteLine(_T("\n--- TimeProvider Demonstration ---"));

    auto provider = TimeProvider::GetSystem();

    int64_t start = provider->GetTimestamp();
    Console::WriteLine(_T("Starting operation at high-res timestamp..."));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    TimeSpan elapsed = provider->GetElapsedTime(start);
    Console::Write(_T("Operation took approx: "));
    Console::Write(elapsed.GetTotalMilliseconds());
    Console::WriteLine(_T(" ms"));
}

void DemonstrateStopwatch() {
    Console::WriteLine(_T("\n--- Stopwatch Demonstration ---"));

    Stopwatch sw = Stopwatch::StartNew();
    Console::WriteLine(_T("Stopwatch started..."));

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    sw.Stop();
    Console::Write(_T("Elapsed milliseconds: "));
    Console::WriteLine(sw.ElapsedMilliseconds());

    sw.Restart();
    Console::WriteLine(_T("Stopwatch restarted..."));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Console::Write(_T("Current elapsed: "));
    Console::Write(sw.Elapsed().GetTotalMilliseconds());
    Console::WriteLine(_T(" ms"));
}

void DemonstrateStringBuilder() {
    Console::WriteLine(_T("\n--- StringBuilder Demonstration ---"));

    StringBuilder sb;
    sb.Append(_T("Items: "))
      .Append(10)
      .Append(_T(", Price: "))
      .Append(19.99)
      .Append(_T(", Available: "))
      .Append(true);

    Console::WriteLine(sb.ToString());

    sb.Clear();
    sb.AppendLine(_T("Shopping List:"))
      .AppendLine(_T("- Apples"))
      .AppendLine(_T("- Bananas"));

    Console::Write(sb.ToString());
}

void DemonstrateList() {
    Console::WriteLine(_T("\n--- List<T> Demonstration ---"));

    List<String> fruits = { _T("Apple"), _T("Banana"), _T("Cherry") };
    fruits.Add(_T("Date"));

    Console::WriteLine(_T("Fruit List:"));
    for (const auto& fruit : fruits) {
        Console::Write(_T("  "));
        Console::WriteLine(fruit);
    }

    fruits.Sort();
    Console::WriteLine(_T("Sorted Fruits:"));
    for (const auto& fruit : fruits) {
        Console::Write(_T("  "));
        Console::WriteLine(fruit);
    }
}

void DemonstrateDictionary() {
    Console::WriteLine(_T("\n--- Dictionary Demonstration ---"));

    Dictionary<String, String> config;
    config.Add(_T("AppName"), _T("DotNetDupe Demo"));
    config.Add(_T("Version"), _T("1.0.0"));

    Console::WriteLine(_T("Config settings:"));
    for (auto const& [key, val] : config) {
        Console::Write(_T("  "));
        Console::Write(key);
        Console::Write(_T(": "));
        Console::WriteLine(val);
    }
}

void DemonstrateGuid() {
    Console::WriteLine(_T("\n--- Guid Demonstration ---"));

    Guid id = Guid::NewGuid();
    Console::Write(_T("New Guid: "));
    Console::WriteLine(id.ToString());

    Guid empty = Guid::Empty;
    Console::Write(_T("Empty Guid: "));
    Console::WriteLine(empty.ToString());
}

void DemonstrateArray() {
    Console::WriteLine(_T("\n--- Array Demonstration ---"));
    Array<int> arr(5);
    for (int i = 0; i < 5; i++) arr [i] = i + 1;

    for (int i = 0; i < arr.GetLength(); i++) {
        Console::Write(arr [i]);
        Console::Write(_T(" "));
    }
    Console::WriteLine();
}

void DemonstrateFile() {
    Console::WriteLine(_T("\n--- File Demonstration ---"));
    const String testFile(_T("test.txt"));
    const String content(_T("Hello, World!"));
    File::WriteAllText(testFile, content);
    String readContent = File::ReadAllText(testFile);
    Console::Write(_T("Read from file: "));
    Console::WriteLine(readContent);
    File::Delete(testFile);
}

int main() {
    DemonstrateConsole();
    DemonstrateBitConverter();
    DemonstrateArray();
    DemonstrateFile();
    DemonstrateConvert();
    DemonstrateTimeProvider();
    DemonstrateStopwatch();
    DemonstrateStringBuilder();
    DemonstrateList();
    DemonstrateDictionary();
    DemonstrateGuid();

    Console::WriteLine(_T("\n--- Demonstration Complete ---"));
    return 0;
}
