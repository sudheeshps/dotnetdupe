#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

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

            struct RdpSessionInfo {
                unsigned long uSessionId;
                String sSessionName;
                String sUserName;
                String sDomainName;
                String sClientName;
                String sClientIpAddress;
                RdpSessionState eState;
                bool bIsRdpSession;

                RdpSessionInfo()
                    : uSessionId(0),
                      eState(RdpSessionState::Unknown),
                      bIsRdpSession(false) {}
            };

            class TerminalSession : public Object {
            public:
                DOTNETDUPE_API TerminalSession();
                DOTNETDUPE_API virtual ~TerminalSession();

                DOTNETDUPE_API static Collections::Generic::List<RdpSessionInfo> GetSessions();
                DOTNETDUPE_API static Collections::Generic::List<RdpSessionInfo> GetActiveSessions();
                DOTNETDUPE_API static Collections::Generic::List<RdpSessionInfo> GetDisconnectedSessions();

            private:
#if defined(_WIN32)
                static void QuerySessionDetails(unsigned long uSessionId, const String& sSessionName, RdpSessionState eState, RdpSessionInfo& info);
                static String QueryWtsString(unsigned long uSessionId, unsigned int eInfoClass);
                static RdpSessionState ConvertWtsState(int iState);
#endif
            };

        }
    }
}
