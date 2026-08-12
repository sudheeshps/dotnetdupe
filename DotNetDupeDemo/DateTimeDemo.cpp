#include <iostream>
#include "System/DateTime.h"
#include "System/String.h"

using namespace DotNetDupe::System;

void RunDateTimeDemo() {
    std::cout << "--- DateTime Demo ---\n";
    
    DateTime now = DateTime::Now();
    std::cout << "Current Time (Local): " << now.ToString().GetRawString() << "\n";
    
    DateTime utcNow = DateTime::UtcNow();
    std::cout << "Current Time (UTC): " << utcNow.ToString().GetRawString() << "\n";
    
    DateTime customDate(2025, 1, 1, 12, 0, 0);
    std::cout << "Custom Date: " << customDate.ToString().GetRawString() << "\n";
    
    DateTime addedDays = customDate.AddDays(10);
    std::cout << "Custom Date + 10 Days: " << addedDays.ToString().GetRawString() << "\n";
    
    if (DateTime::IsLeapYear(2024)) {
        std::cout << "2024 is a leap year.\n";
    }
}
