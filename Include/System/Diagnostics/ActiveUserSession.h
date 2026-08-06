#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            struct UserSessionInfo {
                unsigned long uSessionId;
                String sUsername;
                String sPrivilege;
                String sLoginTimestamp;
                String sLogoutTimestamp;
                bool bIsActive;
            };

            class ActiveUserSession : public Object {
            public:
                DOTNETDUPE_API ActiveUserSession();
                DOTNETDUPE_API virtual ~ActiveUserSession();

                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetActiveSessions();
                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetExpiredSessions();
                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetAllSessions();

            private:
#if defined(_WIN32)
                static void EnumerateWin32Sessions(Collections::Generic::List<UserSessionInfo>& lstSessions);
#endif
            };

        }
    }
}
