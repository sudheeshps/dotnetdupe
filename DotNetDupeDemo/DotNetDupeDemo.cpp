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
#include "System/TimeZone.h"
#include "System/TimeZoneInfo.h"
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
    Console::SetForegroundColor(ConsoleColor::Cyan);
    Console::WriteLine(_T("  This is Cyan text"));
    Console::ResetColor();
    
    Console::SetTitle(_T("DotNetDupe Demo Application"));
}

void DemonstrateString() {
    Console::WriteLine(_T("\n--- String Demonstration ---"));
    
    String s1 = _T("   Hello World   ");
    Console::Write(_T("Original: '"));
    Console::Write(s1);
    Console::WriteLine(_T("'"));
    
    Console::Write(_T("Trimmed: '"));
    Console::Write(s1.Trim());
    Console::WriteLine(_T("'"));
    
    Console::Write(_T("Lower: "));
    Console::WriteLine(s1.ToLower());
    
    Console::Write(_T("Upper: "));
    Console::WriteLine(s1.ToUpper());

    String s2 = _T("Apple,Banana,Cherry");
    auto parts = s2.Split(_T(','));
    Console::WriteLine(_T("Splitting 'Apple,Banana,Cherry':"));
    for(const auto& p : parts) {
        Console::Write(_T("  - "));
        Console::WriteLine(p);
    }
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

    // Numeric to String
    String boolStr = Convert::ToString(true);
    Console::Write(_T("Bool true to String: "));
    Console::WriteLine(boolStr);

    // Base conversions
    String hexStr = Convert::ToString(255, 16);
    Console::Write(_T("Int 255 to Hex: "));
    Console::WriteLine(hexStr);
}

void DemonstrateTimeProvider() {
    Console::WriteLine(_T("\n--- TimeProvider Demonstration ---"));

    auto provider = TimeProvider::GetSystem();
    DateTimeOffset now = provider->GetUtcNow();
    
    Console::Write(_T("UTC Ticks: "));
    Console::WriteLine((long long)now.GetTicks());

    int64_t start = provider->GetTimestamp();
    Console::WriteLine(_T("Starting operation..."));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    TimeSpan elapsed = provider->GetElapsedTime(start);
    Console::Write(_T("Operation took: "));
    Console::Write((double)elapsed.GetTotalMilliseconds());
    Console::WriteLine(_T(" ms"));
}

void DemonstrateStopwatch() {
    Console::WriteLine(_T("\n--- Stopwatch Demonstration ---"));

    Stopwatch sw = Stopwatch::StartNew();
    Console::WriteLine(_T("Stopwatch started..."));

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    sw.Stop();
    Console::Write(_T("Elapsed: "));
    Console::Write((double)sw.Elapsed().GetTotalMilliseconds());
    Console::WriteLine(_T(" ms"));
}

void DemonstrateStringBuilder() {
    Console::WriteLine(_T("\n--- StringBuilder Demonstration ---"));

    StringBuilder sb;
    sb.Append(_T("User: "))
      .Append(_T("Gemini"))
      .AppendLine()
      .Append(_T("Status: "))
      .Append(true)
      .AppendLine()
      .Append(_T("Score: "))
      .Append(98.5);

    Console::Write(sb.ToString());
    Console::WriteLine();
}

void DemonstrateCollections() {
    Console::WriteLine(_T("\n--- Collections Demonstration ---"));

    // List
    List<int> numbers = { 5, 2, 8, 1, 9 };
    numbers.Add(4);
    numbers.Sort();
    
    Console::Write(_T("Sorted List: "));
    for (int n : numbers) {
        Console::Write(n);
        Console::Write(_T(" "));
    }
    Console::WriteLine();

    // List Functional
    Console::Write(_T("Numbers > 5: "));
    auto bigNumbers = numbers.FindAll([](int n) { return n > 5; });
    for (int n : bigNumbers) {
        Console::Write(n);
        Console::Write(_T(" "));
    }
    Console::WriteLine();

    // Dictionary
    Dictionary<String, String> meta;
    meta.Add(_T("OS"), _T("Windows"));
    meta.Add(_T("Arch"), _T("x64"));

    Console::WriteLine(_T("Dictionary Meta:"));
    for (auto const& [key, val] : meta) {
        Console::Write(_T("  "));
        Console::Write(key);
        Console::Write(_T(" => "));
        Console::WriteLine(val);
    }
}

void DemonstrateGuid() {
    Console::WriteLine(_T("\n--- Guid Demonstration ---"));
    Console::Write(_T("New Guid: "));
    Console::WriteLine(Guid::NewGuid().ToString());
}

void DemonstrateEnvironment() {
    Console::WriteLine(_T("\n--- Environment Demonstration ---"));
    Console::Write(_T("OS: "));
    Console::WriteLine(Environment::GetOperatingSystem().GetVersionString());
    Console::Write(_T("Processor Count: "));
    Console::WriteLine((int)Environment::GetProcessorCount());
}

void DemonstrateTimeZone() {
    Console::WriteLine(_T("\n--- TimeZone Demonstration ---"));

    TimeZone* current = TimeZone::GetCurrentTimeZone();
    Console::Write(_T("Current TimeZone Standard Name: "));
    Console::WriteLine(current->GetStandardName());
    Console::Write(_T("Current TimeZone Daylight Name: "));
    Console::WriteLine(current->GetDaylightName());

    DateTimeOffset now(638200000000000000LL); // Arbitrary point
    TimeSpan offset = current->GetUtcOffset(now);
    Console::Write(_T("Current UTC Offset (hours): "));
    Console::WriteLine(offset.GetTotalHours());

    TimeZoneInfo local = TimeZoneInfo::Local();
    Console::Write(_T("TimeZoneInfo Local Id: "));
    Console::WriteLine(local.GetId());
    Console::Write(_T("TimeZoneInfo Local Display Name: "));
    Console::WriteLine(local.GetDisplayName());

    TimeZoneInfo utc = TimeZoneInfo::Utc();
    Console::Write(_T("TimeZoneInfo Utc Id: "));
    Console::WriteLine(utc.GetId());
}

int main() {
    DemonstrateConsole();
    DemonstrateString();
    DemonstrateBitConverter();
    DemonstrateConvert();
    DemonstrateTimeProvider();
    DemonstrateStopwatch();
    DemonstrateStringBuilder();
    DemonstrateCollections();
    DemonstrateGuid();
    DemonstrateEnvironment();
    DemonstrateTimeZone();
    
    Console::WriteLine(_T("\n--- Demonstration Complete ---"));
    return 0;
}
