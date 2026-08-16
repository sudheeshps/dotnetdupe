# TerminalSession, RdpSessionInfo &amp; RdpSessionState

**Namespace:** `DotNetDupe::System::Diagnostics`  
**Header:** `#include "System/Diagnostics/TerminalSession.h"`

Enumerates active, disconnected, and remote desktop (RDP) Terminal Services sessions, client names, and client IP addresses.

---

## `RdpSessionState` Enum

```cpp
enum class RdpSessionState {
    Active,
    Connected,
    ConnectQuery,
    Shadow,
    Disconnected,
    Idle,
    Listen,
    Reset,
    Down,
    Init,
    Unknown
};
```

---

## `RdpSessionInfo` Struct

```cpp
struct RdpSessionInfo {
    unsigned long uSessionId;
    String sSessionName;
    String sUserName;
    String sDomainName;
    String sClientName;
    String sClientIpAddress;
    RdpSessionState eState;
    bool bIsRdpSession;
};
```

---

## `TerminalSession` Class

### Syntax
```cpp
class TerminalSession : public Object;
```

---

## Static Methods

### `static List<RdpSessionInfo> GetSessions()`
Enumerates all Terminal Services / RDP sessions regardless of connection state.

### `static List<RdpSessionInfo> GetActiveSessions()`
Enumerates all active connected user desktop sessions.

### `static List<RdpSessionInfo> GetDisconnectedSessions()`
Enumerates disconnected or idle background sessions.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/TerminalSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    auto rdpSessions = TerminalSession::GetSessions();

    Console::WriteLine("Terminal Sessions ({0} total):", rdpSessions.GetCount());
    for (int i = 0; i < rdpSessions.GetCount(); ++i) {
        const auto& s = rdpSessions[i];
        Console::WriteLine(" [Session #{0}] User: {1}\\{2} (RDP: {3}, IP: {4})",
            s.uSessionId,
            s.sDomainName,
            s.sUserName,
            s.bIsRdpSession,
            s.sClientIpAddress);
    }

    return 0;
}
```
