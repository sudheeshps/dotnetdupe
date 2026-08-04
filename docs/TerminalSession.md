# TerminalSession

`TerminalSession` provides APIs to enumerate and inspect active, disconnected, and remote desktop (RDP) sessions on a Windows machine via Terminal Services APIs (`WTSEnumerateSessions`, `WTSQuerySessionInformation`).

## Namespace
`DotNetDupe::System::Diagnostics`

## Header
```cpp
#include "System/Diagnostics/TerminalSession.h"
```

## Struct: RdpSessionInfo
| Property | Type | Description |
| :--- | :--- | :--- |
| `uSessionId` | `unsigned long` | Terminal Services Session ID |
| `sSessionName` | `String` | Session identifier name (e.g. `"Console"`, `"RDP-Tcp#0"`) |
| `sUserName` | `String` | Username logged into the session |
| `sDomainName` | `String` | User domain name |
| `sClientName` | `String` | Remote client machine name |
| `sClientIpAddress` | `String` | Remote client IP address |
| `eState` | `RdpSessionState` | Current state of session (`Active`, `Disconnected`, etc.) |
| `bIsRdpSession` | `bool` | `true` if connected via RDP protocol |

## Static Methods

### GetSessions
```cpp
static List<RdpSessionInfo> GetSessions();
```
Enumerates all running sessions (Console, RDP, Listener) on the local Terminal Server.

### GetActiveSessions
```cpp
static List<RdpSessionInfo> GetActiveSessions();
```
Returns a list of sessions currently in `RdpSessionState::Active` state.

### GetDisconnectedSessions
```cpp
static List<RdpSessionInfo> GetDisconnectedSessions();
```
Returns a list of sessions currently in `RdpSessionState::Disconnected` state.

## Usage Example

```cpp
#include "System/Console.h"
#include "System/Diagnostics/TerminalSession.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Diagnostics;

int main() {
    auto lstSessions = TerminalSession::GetSessions();
    Console::WriteLine("Total Sessions: " + String::ValueOf(lstSessions.GetCount()));

    for (int i = 0; i < lstSessions.GetCount(); ++i) {
        const auto& session = lstSessions[i];
        Console::WriteLine("Session ID: " + String::ValueOf(static_cast<int>(session.uSessionId)));
        Console::WriteLine("  User: " + session.sUserName);
        Console::WriteLine("  Client Name: " + session.sClientName);
    }
    return 0;
}
```
