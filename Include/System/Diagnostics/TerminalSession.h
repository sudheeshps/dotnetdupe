#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            /// \enum RdpSessionState
            /// \brief Represents the operational connection state of a Remote Desktop or Terminal session.
            enum class RdpSessionState {
                Active,         ///< User is logged on and actively interacting with the session.
                Connected,      ///< Client is connected to terminal session but logon not yet completed.
                ConnectQuery,   ///< Session is in process of connecting to client.
                Shadow,         ///< Session is being shadowed by another session.
                Disconnected,   ///< Session is disconnected but state preserved for reconnection.
                Idle,           ///< Session is waiting for connection.
                Listen,         ///< Session is listening for incoming client connections.
                Reset,          ///< Session is being reset.
                Down,           ///< Session is down or non-operational.
                Init,           ///< Session is initializing.
                Unknown         ///< State could not be determined.
            };

            /// \struct RdpSessionInfo
            /// \brief Contains extended telemetry for a Remote Desktop Protocol (RDP) session.
            struct RdpSessionInfo {
                unsigned long uSessionId;   ///< Terminal Services session ID.
                String sSessionName;       ///< Name of the WinStation (e.g. "Console", "RDP-Tcp#0").
                String sUserName;          ///< User account associated with session.
                String sDomainName;        ///< Domain of the authenticated user account.
                String sClientName;        ///< NetBIOS or DNS name of connecting client endpoint.
                String sClientIpAddress;   ///< IPv4 or IPv6 client address.
                RdpSessionState eState;    ///< Current RDP connection state.
                bool bIsRdpSession;        ///< True if session is an active or disconnected RDP remote session.

                /// \brief Default constructor initializing default state.
                RdpSessionInfo()
                    : uSessionId(0),
                      eState(RdpSessionState::Unknown),
                      bIsRdpSession(false) {}
            };

            /// \class TerminalSession
            /// \brief Provides querying and enumeration of Terminal Services and RDP sessions.
            ///
            /// Interfaces with Windows Terminal Services APIs (`wtsapi32.dll`) to discover active,
            /// disconnected, and remote client sessions with client IP and machine names.
            class TerminalSession : public Object {
            public:
                /// \brief Initializes a new instance of TerminalSession.
                DOTNETDUPE_API TerminalSession();

                /// \brief Virtual destructor.
                DOTNETDUPE_API virtual ~TerminalSession();

                /// \brief Enumerates all terminal sessions on the local system.
                /// \return List of all RdpSessionInfo records.
                /// \throws DotNetDupe::System::UnauthorizedAccessException If caller lacks administrative rights.
                DOTNETDUPE_API static Collections::Generic::List<RdpSessionInfo> GetSessions();

                /// \brief Enumerates only actively connected terminal sessions.
                /// \return List of active RdpSessionInfo records.
                DOTNETDUPE_API static Collections::Generic::List<RdpSessionInfo> GetActiveSessions();

                /// \brief Enumerates disconnected terminal sessions available for reconnection.
                /// \return List of disconnected RdpSessionInfo records.
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
