#include "System/Console.h"
#include "System/TimeZone.h"
#include "System/TimeZoneInfo.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateTimeZone() {
    Console::WriteLine("\n--- TimeZone Demonstration ---");

    TimeZone* pCurrent = TimeZone::GetCurrentTimeZone();
    Console::Write("Current TimeZone Standard Name: ");
    Console::WriteLine(pCurrent->GetStandardName());
    Console::Write("Current TimeZone Daylight Name: ");
    Console::WriteLine(pCurrent->GetDaylightName());

    DateTimeOffset dtNow(638200000000000000LL); // Arbitrary point
    TimeSpan tsOffset = pCurrent->GetUtcOffset(dtNow);
    Console::Write("Current UTC Offset (hours): ");
    Console::WriteLine(tsOffset.GetTotalHours());

    TimeZoneInfo objLocal = TimeZoneInfo::Local();
    Console::Write("TimeZoneInfo Local Id: ");
    Console::WriteLine(objLocal.GetId());
    Console::Write("TimeZoneInfo Local Display Name: ");
    Console::WriteLine(objLocal.GetDisplayName());

    TimeZoneInfo objUtc = TimeZoneInfo::Utc();
    Console::Write("TimeZoneInfo Utc Id: ");
    Console::WriteLine(objUtc.GetId());
}
