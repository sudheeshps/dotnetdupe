#include "System/Console.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Security::Principal;

void RunUserPrincipalDemo() {
    Console::WriteLine("=== UserPrincipal & Account Rights Demo ===");

    try {
        UserInfo current = UserPrincipal::GetCurrent();
        Console::Write("Current User: ");
        Console::WriteLine(current.sUsername);

        Console::Write("User Class: ");
        if (current.eUserClass == UserClass::Admin) Console::WriteLine("Admin");
        else if (current.eUserClass == UserClass::System) Console::WriteLine("System");
        else if (current.eUserClass == UserClass::Guest) Console::WriteLine("Guest");
        else Console::WriteLine("Normal");

        Console::WriteLine("Enumerating System Users:");
        auto users = UserPrincipal::EnumerateUsers();
        for (int i = 0; i < users.GetCount(); i++) {
            const auto& user = users[i];
            Console::Write(" - User: ");
            Console::Write(user.sUsername);
            Console::Write(" | Class: ");
            if (user.eUserClass == UserClass::Admin) Console::Write("Admin");
            else if (user.eUserClass == UserClass::System) Console::Write("System");
            else if (user.eUserClass == UserClass::Guest) Console::Write("Guest");
            else Console::Write("Normal");

            Console::Write(" | Groups: ");
            Console::WriteLine(Convert::ToString(user.lstGroups.GetCount()));
        }
    } catch (const std::exception& ex) {
        Console::Write("Exception in UserPrincipalDemo: ");
        Console::WriteLine(ex.what());
    }
}
