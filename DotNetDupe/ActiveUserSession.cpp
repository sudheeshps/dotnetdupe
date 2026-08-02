#include "pch.h"
#include "System/Diagnostics/ActiveUserSession.h"

#if defined(_WIN32)
#include <windows.h>
#include <wtsapi32.h>
#pragma comment(lib, "wtsapi32.lib")
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            ActiveUserSession::ActiveUserSession() {}
            ActiveUserSession::~ActiveUserSession() {}

#if defined(_WIN32)
            static void EnumerateWin32Sessions(Collections::Generic::List<UserSessionInfo>& lstSessions) {
                WTS_SESSION_INFOW* pSessionInfo = NULL;
                DWORD dwSessionCount = 0;
                if (::WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwSessionCount) && pSessionInfo) {
                    for (DWORD i = 0; i < dwSessionCount; ++i) {
                        LPWSTR pBuffer = NULL; DWORD dwBytes = 0;
                        String sUsername("SYSTEM");
                        if (::WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, pSessionInfo[i].SessionId, WTSUserName, &pBuffer, &dwBytes) && pBuffer && dwBytes > 2) {
                            char szUserUtf8[256] = { 0 };
                            ::WideCharToMultiByte(CP_UTF8, 0, pBuffer, -1, szUserUtf8, sizeof(szUserUtf8), NULL, NULL);
                            sUsername = String(szUserUtf8);
                            ::WTSFreeMemory(pBuffer);
                        }

                        UserSessionInfo session;
                        session.uSessionId = pSessionInfo[i].SessionId;
                        session.sUsername = sUsername;
                        session.bIsActive = (pSessionInfo[i].State == WTSActive);
                        session.sPrivilege = session.bIsActive ? String("Administrator (Active Terminal)") : String("Standard User");
                        session.sLoginTimestamp = String("Active Session");
                        session.sLogoutTimestamp = session.bIsActive ? String("Active Session") : String("Session Disconnected");

                        lstSessions.Add(session);
                    }
                    ::WTSFreeMemory(pSessionInfo);
                }
            }
#endif

            Collections::Generic::List<UserSessionInfo> ActiveUserSession::GetAllSessions() {
                Collections::Generic::List<UserSessionInfo> lstSessions;
#if defined(_WIN32)
                EnumerateWin32Sessions(lstSessions);
#endif
                return lstSessions;
            }

            Collections::Generic::List<UserSessionInfo> ActiveUserSession::GetActiveSessions() {
                auto lstAll = GetAllSessions();
                Collections::Generic::List<UserSessionInfo> lstActive;
                for (int i = 0; i < lstAll.GetCount(); i++) {
                    if (lstAll[i].bIsActive) lstActive.Add(lstAll[i]);
                }
                return lstActive;
            }

            Collections::Generic::List<UserSessionInfo> ActiveUserSession::GetExpiredSessions() {
                auto lstAll = GetAllSessions();
                Collections::Generic::List<UserSessionInfo> lstExpired;
                for (int i = 0; i < lstAll.GetCount(); i++) {
                    if (!lstAll[i].bIsActive) lstExpired.Add(lstAll[i]);
                }
                return lstExpired;
            }

        }
    }
}
