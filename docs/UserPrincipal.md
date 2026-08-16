# UserPrincipal, UserInfo &amp; UserClass

**Namespace:** `DotNetDupe::System::Security::Principal`  
**Header:** `#include "System/Security/Principal/UserPrincipal.h"`

Provides cross-platform user account enumeration, group memberships, and administrative privilege inspection.

---

## `UserClass` Enum

```cpp
enum class UserClass {
    Guest = 0,
    Normal = 1,
    Admin = 2,
    System = 3
};
```

---

## `UserInfo` Struct

```cpp
struct UserInfo {
    String sUsername;
    String sDomain;
    String sSidOrUid;
    UserClass eUserClass;
    bool bIsDisabled;
    bool bIsPasswordRequired;
    bool bIsAccountLocked;
    List<String> lstGroups;
    List<String> lstPermissions;
};
```

---

## `UserPrincipal` Class

### Syntax
```cpp
class UserPrincipal : public Object;
```

---

## Static Methods

### `static UserInfo GetCurrent()`
Gets account and privilege information for the currently executing process user.

### `static UserInfo GetUser(const String& sUsername)`
Retrieves account and security group details for a specified username.

### `static List<UserInfo> EnumerateUsers()`
Enumerates all local user accounts registered on the system.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Security/Principal/UserPrincipal.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Security::Principal;

int main() {
    UserInfo current = UserPrincipal::GetCurrent();
    Console::WriteLine("Current User: {0}\\{1} (SID: {2})", 
        current.sDomain, current.sUsername, current.sSidOrUid);
    Console::WriteLine("Is Administrator: {0}", (current.eUserClass == UserClass::Admin));

    Console::WriteLine("Group Memberships:");
    for (int i = 0; i < current.lstGroups.GetCount(); ++i) {
        Console::WriteLine(" - {0}", current.lstGroups[i]);
    }

    return 0;
}
```
