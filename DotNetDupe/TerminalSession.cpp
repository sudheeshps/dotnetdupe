#include "pch.h"
#include "System/Diagnostics/TerminalSession.h"
#include "System/Utils/StringConvert.h"
#include <vector>

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <wtsapi32.h>
#pragma comment(lib, "wtsapi32.lib")
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            TerminalSession::TerminalSession() {}
            TerminalSession::~TerminalSession() {}

#if defined(_WIN32)
            RdpSessionState TerminalSession::ConvertWtsState(int iState) {
                switch (iState) {
                case WTSActive: return RdpSessionState::Active;
                case WTSConnected: return RdpSessionState::Connected;
                case WTSConnectQuery: return RdpSessionState::ConnectQuery;
                case WTSShadow: return RdpSessionState::Shadow;
                case WTSDisconnected: return RdpSessionState::Disconnected;
                case WTSIdle: return RdpSessionState::Idle;
                case WTSListen: return RdpSessionState::Listen;
                case WTSReset: return RdpSessionState::Reset;
                case WTSDown: return RdpSessionState::Down;
                case WTSInit: return RdpSessionState::Init;
                default: return RdpSessionState::Unknown;
                }
            }

            String TerminalSession::QueryWtsString(unsigned long uSessionId, unsigned int eInfoClass) {
                LPWSTR pBuffer = NULL;
                DWORD dwBytesReturned = 0;
                String sResult;

                if (::WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, uSessionId, static_cast<WTS_INFO_CLASS>(eInfoClass), &pBuffer, &dwBytesReturned)) {
                    if (pBuffer != NULL && dwBytesReturned > 0) {
                        sResult = String(pBuffer);
                    }
                    ::WTSFreeMemory(pBuffer);
                }

                return sResult;
            }

            void TerminalSession::QuerySessionDetails(unsigned long uSessionId, const String& sSessionName, RdpSessionState eState, RdpSessionInfo& info) {
                info.uSessionId = uSessionId;
                info.sSessionName = sSessionName;
                info.eState = eState;
                info.sUserName = QueryWtsString(uSessionId, WTSUserName);
                info.sDomainName = QueryWtsString(uSessionId, WTSDomainName);
                info.sClientName = QueryWtsString(uSessionId, WTSClientName);
                info.sClientIpAddress = QueryWtsString(uSessionId, WTSClientAddress);
                info.bIsRdpSession = !info.sClientName.IsEmpty() || info.sSessionName.Contains("RDP");
            }
#endif

            Collections::Generic::List<RdpSessionInfo> TerminalSession::GetSessions() {
                Collections::Generic::List<RdpSessionInfo> lstResult;
#if defined(_WIN32)
                WTS_SESSION_INFOW* pSessionInfo = NULL;
                DWORD dwCount = 0;

                if (::WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount)) {
                    for (DWORD i = 0; i < dwCount; ++i) {
                        RdpSessionInfo info;
                        String sName = (pSessionInfo[i].pWinStationName != NULL) ? String(pSessionInfo[i].pWinStationName) : String("");
                        RdpSessionState eState = ConvertWtsState(pSessionInfo[i].State);

                        QuerySessionDetails(pSessionInfo[i].SessionId, sName, eState, info);
                        lstResult.Add(info);
                    }

                    ::WTSFreeMemory(pSessionInfo);
                }
#endif
                return lstResult;
            }

            Collections::Generic::List<RdpSessionInfo> TerminalSession::GetActiveSessions() {
                auto lstAll = GetSessions();
                Collections::Generic::List<RdpSessionInfo> lstActive;

                for (int i = 0; i < lstAll.GetCount(); ++i) {
                    if (lstAll[i].eState == RdpSessionState::Active) {
                        lstActive.Add(lstAll[i]);
                    }
                }

                return lstActive;
            }

            Collections::Generic::List<RdpSessionInfo> TerminalSession::GetDisconnectedSessions() {
                auto lstAll = GetSessions();
                Collections::Generic::List<RdpSessionInfo> lstDisconnected;

                for (int i = 0; i < lstAll.GetCount(); ++i) {
                    if (lstAll[i].eState == RdpSessionState::Disconnected) {
                        lstDisconnected.Add(lstAll[i]);
                    }
                }

                return lstDisconnected;
            }

        }
    }
}
