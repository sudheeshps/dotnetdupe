### class `UserPrincipal`

Provides APIs for discovering and enumerating system user accounts, user classification (`Admin`, `Normal`, `Guest`, `System`), account flags, group memberships, and security rights on Windows and Linux systems.

#### Enums & Structs

##### `enum class UserClass`
- `Guest`: Limited or guest user account.
- `Normal`: Standard user account.
- `Admin`: Administrator / root user account.
- `System`: Internal service or daemon user account.

##### `struct UserInfo`
- `String sUsername`: Account name.
- `String sDomain`: User domain or local machine indicator.
- `String sSidOrUid`: Windows SID string or Linux UID.
- `UserClass eUserClass`: Classification of the user account.
- `bool bIsDisabled`: Whether the account is disabled.
- `bool bIsPasswordRequired`: Whether a password is required for authentication.
- `bool bIsAccountLocked`: Whether the account is currently locked out.
- `List<String> lstGroups`: List of group memberships associated with the user account.
- `List<String> lstPermissions`: List of user rights, group privileges, and security permissions.

#### Methods

##### `static List<UserInfo> EnumerateUsers()`

Enumerates system accounts on Windows (via `NetUserEnum`) and Linux (via `getpwent`).

##### `static UserInfo GetUser(const String& sUsername)`

Fetches account details, privileges, and group memberships for a specified username.

##### `static UserInfo GetCurrent()`

Retrieves information for the currently executing user context.

---

## Code Example

```cpp
#include "System/Console.h"
#include "System/Security/Principal/UserPrincipal.h"
#include "System/Convert.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Security::Principal;

int main() {
    // Retrieve current user account details
    UserInfo currentUser = UserPrincipal::GetCurrent();
    Console::Write("Current User: ");
    Console::WriteLine(currentUser.sUsername);

    Console::Write("Account Class: ");
    if (currentUser.eUserClass == UserClass::Admin) {
        Console::WriteLine("Administrator");
    } else {
        Console::WriteLine("Standard User");
    }

    // Enumerate all system user accounts
    Console::WriteLine("\nSystem Accounts:");
    auto users = UserPrincipal::EnumerateUsers();
    for (int i = 0; i < users.GetCount(); i++) {
        const auto& user = users[i];
        Console::Write("User: ");
        Console::Write(user.sUsername);
        Console::Write(" | Groups Count: ");
        Console::WriteLine(Convert::ToString(user.lstGroups.GetCount()));
    }

    return 0;
}
```
