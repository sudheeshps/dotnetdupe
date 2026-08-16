# ActiveUserSession &amp; UserSessionInfo

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/ActiveUserSession.h"`

Provides cross-platform user login session discovery, auditing active desktop sessions, logon privilege levels, and logon/logout timestamps.

---

## `UserSessionInfo` Struct

```cpp
struct UserSessionInfo {
    unsigned long uSessionId;
    String sUsername;
    String sPrivilege;
    String sLoginTimestamp;
    String sLogoutTimestamp;
    bool bIsActive;
};
```

---

## `ActiveUserSession` Class

### Syntax
```cpp
class ActiveUserSession : public Object;
```

---

## Static Methods

### `static List<UserSessionInfo> GetActiveSessions()`
Enumerates all currently active logged-in user sessions on the system.

### `static List<UserSessionInfo> GetExpiredSessions()`
Enumerates logged-off or terminated historical user sessions.

### `static List<UserSessionInfo> GetAllSessions()`
Enumerates all recorded user sessions regardless of active or terminated state.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/ActiveUserSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    auto activeSessions = ActiveUserSession::GetActiveSessions();

    Console::WriteLine("Active User Sessions ({0} found):", activeSessions.GetCount());
    for (int i = 0; i < activeSessions.GetCount(); ++i) {
        const auto& session = activeSessions[i];
        Console::WriteLine(" [Session {0}] User: {1}, Privilege: {2}, Logged On: {3}",
            session.uSessionId,
            session.sUsername,
            session.sPrivilege,
            session.sLoginTimestamp);
    }

    return 0;
}
```
