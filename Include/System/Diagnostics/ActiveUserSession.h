#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            /// \struct UserSessionInfo
            /// \brief Encapsulates details about an operating system interactive user session.
            struct UserSessionInfo {
                unsigned long uSessionId;   ///< Terminal Services session identifier.
                String sUsername;           ///< Account name of the logged-on user.
                String sPrivilege;          ///< User privilege level or role description.
                String sLoginTimestamp;     ///< Formatted timestamp of when session was established.
                String sLogoutTimestamp;    ///< Formatted timestamp of session disconnection or active status.
                bool bIsActive;             ///< True if session is actively receiving user input.
            };

            /// \class ActiveUserSession
            /// \brief Provides system inspection facilities for active and expired user logon sessions.
            ///
            /// Interrogates Windows Terminal Services (WTS API via `WTSEnumerateSessionsW` and
            /// `WTSQuerySessionInformationW`) to enumerate logged-in console and remote desktop users.
            ///
            /// \note Requires appropriate administrative privileges on Windows to query details for other users.
            class ActiveUserSession : public Object {
            public:
                /// \brief Initializes a new instance of ActiveUserSession.
                DOTNETDUPE_API ActiveUserSession();

                /// \brief Virtual destructor for polymorphic cleanup.
                DOTNETDUPE_API virtual ~ActiveUserSession();

                /// \brief Enumerates all sessions currently in the active interactive state.
                /// \return List of active UserSessionInfo structures.
                /// \throws DotNetDupe::System::UnauthorizedAccessException If access is denied querying sessions.
                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetActiveSessions();

                /// \brief Enumerates all disconnected or inactive user sessions.
                /// \return List of expired or disconnected UserSessionInfo structures.
                /// \throws DotNetDupe::System::UnauthorizedAccessException If access is denied querying sessions.
                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetExpiredSessions();

                /// \brief Enumerates all user sessions regardless of active state.
                /// \return List of all UserSessionInfo structures found on the machine.
                /// \throws DotNetDupe::System::UnauthorizedAccessException If access is denied querying sessions.
                DOTNETDUPE_API static Collections::Generic::List<UserSessionInfo> GetAllSessions();

            private:
#if defined(_WIN32)
                /// \brief Native Windows implementation querying WTS session table.
                /// \param lstSessions Output list populated with session details.
                static void EnumerateWin32Sessions(Collections::Generic::List<UserSessionInfo>& lstSessions);
#endif
            };

        }
    }
}
