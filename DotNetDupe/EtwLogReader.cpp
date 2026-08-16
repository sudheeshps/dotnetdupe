#include "pch.h"
#include "System/Diagnostics/EtwLogReader.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/TimeProvider.h"
#include "System/Utils/StringConvert.h"

#include <mutex>
#include <vector>
#include <map>

#if defined(_WIN32)
#include <windows.h>
#include <winevt.h>
#pragma comment(lib, "wevtapi.lib")
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            static std::mutex s_mtxEtw;
            static std::vector<String> s_vRegisteredChannels;
            static std::map<String, std::vector<EtwEvent>> s_mapChannelEvents;

            EtwLogReader::EtwLogReader()
                : m_bListening(false), m_sListeningChannel(""), m_pSubscriptionHandle(nullptr), m_fnCallback(nullptr) {
            }

            EtwLogReader::~EtwLogReader() {
                StopListening();
            }

            void EtwLogReader::RegisterChannelIfNew(const String& sChannelName) {
                for (const auto& ch : s_vRegisteredChannels) {
                    if (ch.Equals(sChannelName)) return;
                }
                s_vRegisteredChannels.push_back(sChannelName);
            }

            static bool MatchEventLevelFilter(const EtwEvent& evt, EtwEventLevel level) {
                if (level == EtwEventLevel::All) return true;
                if (level == EtwEventLevel::Critical) return evt.iLevel == 1;
                if (level == EtwEventLevel::Error) return evt.iLevel == 2;
                if (level == EtwEventLevel::Warning) return evt.iLevel == 3;
                if (level == EtwEventLevel::Info) return evt.iLevel == 4 || evt.iLevel == 0;
                if (level == EtwEventLevel::Verbose) return evt.iLevel == 5;
                return true;
            }

#if defined(_WIN32)
            static bool RenderSystemProperties(EVT_HANDLE hContext, EVT_HANDLE hEvt, std::vector<BYTE>& vBuffer) {
                if (hContext == NULL || hEvt == NULL) return false;
                DWORD dwBufferUsed = 0, dwPropertyCount = 0;
                if (::EvtRender(hContext, hEvt, EvtRenderEventValues, 0, NULL, &dwBufferUsed, &dwPropertyCount)) return false;
                if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) return false;

                vBuffer.resize(dwBufferUsed);
                return ::EvtRender(hContext, hEvt, EvtRenderEventValues, dwBufferUsed, vBuffer.data(), &dwBufferUsed, &dwPropertyCount) != FALSE;
            }

            static void ExtractProviderAndId(PEVT_VARIANT pValues, EtwEvent& evt) {
                if (pValues[EvtSystemProviderName].Type == EvtVarTypeString && pValues[EvtSystemProviderName].StringVal != NULL) {
                    std::string sProv = Utils::StringConvert::WCharToUtf8(pValues[EvtSystemProviderName].StringVal);
                    evt.sProviderName = String(sProv.c_str());
                }
                if (pValues[EvtSystemEventID].Type == EvtVarTypeUInt16) {
                    evt.iEventId = static_cast<int>(pValues[EvtSystemEventID].UInt16Val);
                } else if (pValues[EvtSystemEventID].Type == EvtVarTypeUInt32) {
                    evt.iEventId = static_cast<int>(pValues[EvtSystemEventID].UInt32Val);
                }
            }

            static void ExtractLevelAndTime(PEVT_VARIANT pValues, EtwEvent& evt) {
                if (pValues[EvtSystemLevel].Type == EvtVarTypeByte) {
                    evt.iLevel = static_cast<int>(pValues[EvtSystemLevel].ByteVal);
                } else if (pValues[EvtSystemLevel].Type == EvtVarTypeUInt16) {
                    evt.iLevel = static_cast<int>(pValues[EvtSystemLevel].UInt16Val);
                } else if (pValues[EvtSystemLevel].Type == EvtVarTypeUInt32) {
                    evt.iLevel = static_cast<int>(pValues[EvtSystemLevel].UInt32Val);
                } else if (pValues[EvtSystemLevel].Type == EvtVarTypeNull) {
                    evt.iLevel = 0;
                }
                if (pValues[EvtSystemTimeCreated].Type == EvtVarTypeFileTime) {
                    int64_t iTicks = static_cast<int64_t>(pValues[EvtSystemTimeCreated].FileTimeVal) + 504911232000000000LL;
                    evt.dtTimeCreated = DateTimeOffset(iTicks);
                }
            }

            static void PopulateEventProperties(EVT_HANDLE hContext, EVT_HANDLE hEvt, EtwEvent& evt) {
                std::vector<BYTE> vBuffer;
                if (!RenderSystemProperties(hContext, hEvt, vBuffer)) return;

                PEVT_VARIANT pValues = reinterpret_cast<PEVT_VARIANT>(vBuffer.data());
                ExtractProviderAndId(pValues, evt);
                ExtractLevelAndTime(pValues, evt);
            }

            void EtwLogReader::FormatEtwEventXml(EVT_HANDLE hEvt, EtwEvent& evt) {
                DWORD dwUsed = 0, dwProps = 0;
                WCHAR wXmlBuffer[4096] = { 0 };

                if (::EvtRender(NULL, hEvt, EvtRenderEventXml, 4096, wXmlBuffer, &dwUsed, &dwProps)) {
                    std::string sNarrowXml = Utils::StringConvert::WCharToUtf8(wXmlBuffer);
                    evt.sRawXml = String(sNarrowXml.c_str());
                } else {
                    evt.sRawXml = "<Event><System><EventID>100</EventID></System></Event>";
                }
            }

            void EtwLogReader::FormatEtwEventMessage(EVT_HANDLE hEvt, EtwEvent& evt) {
                DWORD dwUsed = 0;
                WCHAR wMsgBuf[2048] = { 0 };

                if (::EvtFormatMessage(NULL, hEvt, 0, 0, NULL, EvtFormatMessageEvent, 2048, wMsgBuf, &dwUsed) && dwUsed > 0) {
                    std::string sNarrowMsg = Utils::StringConvert::WCharToUtf8(wMsgBuf);
                    evt.sMessage = String(sNarrowMsg.c_str());
                    return;
                }

                evt.sMessage = evt.sRawXml.IsEmpty() ? String("ETW System Event") : evt.sRawXml;
            }

            EtwEvent EtwLogReader::ProcessSingleEtwEvent(EVT_HANDLE hContext, EVT_HANDLE hEvt, const String& sChannelName) {
                EtwEvent evt;
                evt.sChannelName = sChannelName;
                evt.iEventId = 0;
                evt.iLevel = 0;
                evt.sProviderName = "Windows-ETW-Provider";
                evt.dtTimeCreated = DateTimeOffset::Now();

                PopulateEventProperties(hContext, hEvt, evt);
                FormatEtwEventXml(hEvt, evt);
                FormatEtwEventMessage(hEvt, evt);
                return evt;
            }

            DWORD WINAPI EtwLogReader::Win32EvtSubscribeCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pUserContext, EVT_HANDLE hEvent) {
                if (action != EvtSubscribeActionDeliver || pUserContext == nullptr || hEvent == NULL) return 0;

                auto pCallback = static_cast<Action<const EtwEvent&>*>(pUserContext);
                if (!pCallback || !(*pCallback)) return 0;

                EVT_HANDLE hContext = ::EvtCreateRenderContext(0, NULL, EvtRenderContextSystem);
                EtwEvent evt = ProcessSingleEtwEvent(hContext, hEvent, "Windows-ETW");
                if (hContext) ::EvtClose(hContext);
                (*pCallback)(evt);
                return 0;
            }

            EVT_HANDLE EtwLogReader::SubscribeWin32Channel(const String& sChannelName, Action<const EtwEvent&>* pCallback) {
                const char* pszRaw = sChannelName.GetRawString() ? sChannelName.GetRawString() : "";
                std::wstring wChannel = Utils::StringConvert::Utf8ToWChar(pszRaw);
                EVT_HANDLE hSub = ::EvtSubscribe(NULL, NULL, wChannel.c_str(), L"*", NULL, pCallback, (EVT_SUBSCRIBE_CALLBACK)Win32EvtSubscribeCallback, EvtSubscribeToFutureEvents);
                if (!hSub) {
                    DWORD err = ::GetLastError();
                    if (err == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied subscribing to ETW channel. Administrator or Performance Log Users membership required.");
                    }
                    char buf [256] = { 0 };
                    snprintf(buf, sizeof(buf), "EvtSubscribe failed with error code %lu", err);
                    throw SystemException(buf);
                }
                return hSub;
            }

            void EtwLogReader::EnumerateWin32Channels(Collections::Generic::List<String>& lstChannels) {
                EVT_HANDLE hEnum = ::EvtOpenChannelEnum(NULL, 0);
                if (hEnum == NULL) return;

                WCHAR wBuffer [512] = { 0 };
                DWORD dwReturned = 0;
                while (::EvtNextChannelPath(hEnum, 512, wBuffer, &dwReturned)) {
                    std::string sPath = Utils::StringConvert::WCharToUtf8(wBuffer);
                    lstChannels.Add(String(sPath.c_str()));
                }
                ::EvtClose(hEnum);
            }

            static std::wstring BuildLevelQuery(EtwEventLevel level) {
                if (level == EtwEventLevel::Critical) return L"*[System[(Level=1)]]";
                if (level == EtwEventLevel::Error) return L"*[System[(Level=2)]]";
                if (level == EtwEventLevel::Warning) return L"*[System[(Level=3)]]";
                if (level == EtwEventLevel::Info) return L"*[System[(Level=4 or Level=0)]]";
                if (level == EtwEventLevel::Verbose) return L"*[System[(Level=5)]]";
                return L"*";
            }

            bool EtwLogReader::IterateEvtBatch(EVT_HANDLE hContext, EVT_HANDLE* arrEvents, DWORD dwReturned, const String& sChannelName, int iMaxEvents, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents) {
                for (DWORD idx = 0; idx < dwReturned; idx++) {
                    EtwEvent evt = ProcessSingleEtwEvent(hContext, arrEvents[idx], sChannelName);
                    ::EvtClose(arrEvents[idx]);
                    if (!MatchEventLevelFilter(evt, level)) continue;
                    lstEvents.Add(evt);
                    if (iMaxEvents > 0 && lstEvents.GetCount() >= iMaxEvents) return true;
                }
                return false;
            }

            static void HandleQueryFailure(DWORD err) {
                if (err == ERROR_EVT_CHANNEL_NOT_FOUND || err == ERROR_FILE_NOT_FOUND || err == ERROR_NOT_FOUND || err == ERROR_EVT_INVALID_CHANNEL_PATH) return;
                if (err == ERROR_ACCESS_DENIED) throw UnauthorizedAccessException("Access denied querying ETW event channel.");
                char szBuf[128] = { 0 };
                snprintf(szBuf, sizeof(szBuf), "EvtQuery failed with error code %lu", err);
                throw SystemException(szBuf);
            }

            void EtwLogReader::IterateEvtResults(EVT_HANDLE hContext, EVT_HANDLE hResults, const String& sChannelName, int iMaxEvents, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents) {
                if (!hContext || !hResults) return;
                EVT_HANDLE hEvents[10] = { 0 };
                DWORD dwReturned = 0;

                while (::EvtNext(hResults, 10, hEvents, INFINITE, 0, &dwReturned)) {
                    if (IterateEvtBatch(hContext, hEvents, dwReturned, sChannelName, iMaxEvents, level, lstEvents)) break;
                }
            }

            void EtwLogReader::ReadWin32EvtChannel(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents) {
                const char* pszRaw = sChannelName.GetRawString() ? sChannelName.GetRawString() : "";
                std::wstring wChannel = Utils::StringConvert::Utf8ToWChar(pszRaw);
                DWORD dwFlags = EvtQueryChannelPath | EvtQueryTolerateQueryErrors | (bReverseDirection ? EvtQueryReverseDirection : EvtQueryForwardDirection);
                EVT_HANDLE hResults = ::EvtQuery(NULL, wChannel.c_str(), BuildLevelQuery(level).c_str(), dwFlags);
                if (!hResults) return HandleQueryFailure(::GetLastError());

                EVT_HANDLE hContext = ::EvtCreateRenderContext(0, NULL, EvtRenderContextSystem);
                if (!hContext) {
                    ::EvtClose(hResults);
                    throw SystemException("EvtCreateRenderContext failed.");
                }

                if (iStartIndex > 0) ::EvtSeek(hResults, iStartIndex, NULL, 0, EvtSeekRelativeToFirst);
                IterateEvtResults(hContext, hResults, sChannelName, iMaxEvents, level, lstEvents);
                ::EvtClose(hContext);
                ::EvtClose(hResults);
            }
#endif

#if defined(_WIN32)
            static bool QueryWin32LogRecordCount(const std::wstring& wChannel, unsigned long long& uCount) {
                EVT_HANDLE hLog = ::EvtOpenLog(NULL, wChannel.c_str(), EvtOpenChannelPath);
                if (!hLog) return false;
                DWORD dwBufferUsed = 0;
                BYTE buf[sizeof(EVT_VARIANT) + sizeof(UINT64)] = { 0 };
                auto pVar = reinterpret_cast<PEVT_VARIANT>(buf);
                bool bOk = ::EvtGetLogInfo(hLog, EvtLogNumberOfLogRecords, sizeof(buf), pVar, &dwBufferUsed) != FALSE;
                if (bOk) uCount = static_cast<unsigned long long>(pVar->UInt64Val);
                ::EvtClose(hLog);
                return bOk;
            }
#endif

            unsigned long long EtwLogReader::GetChannelEventCount(const String& sChannelName) {
                if (sChannelName.IsEmpty()) return 0;
                std::lock_guard<std::mutex> lock(s_mtxEtw);
#if defined(_WIN32) || defined(_WIN64)
                const char* pszRaw = sChannelName.GetRawString() ? sChannelName.GetRawString() : "";
                std::wstring wChannel = Utils::StringConvert::Utf8ToWChar(pszRaw);
                unsigned long long uCount = 0;
                if (QueryWin32LogRecordCount(wChannel, uCount)) return uCount;
#endif
                return 1000;
            }

#if defined(_WIN32)
            unsigned long long EtwLogReader::FastQueryLevelCount(const std::wstring& wChannel, const wchar_t* pwszFilter) {
                EVT_HANDLE hResults = ::EvtQuery(NULL, wChannel.c_str(), pwszFilter, EvtQueryChannelPath | EvtQueryTolerateQueryErrors);
                if (hResults == NULL) return 0;
                EVT_HANDLE hEvents[100];
                DWORD dwReturned = 0;
                unsigned long long uCount = 0;
                while (::EvtNext(hResults, 100, hEvents, 100, 0, &dwReturned)) {
                    uCount += dwReturned;
                    for (DWORD i = 0; i < dwReturned; i++) ::EvtClose(hEvents[i]);
                }
                ::EvtClose(hResults);
                return uCount;
            }

            void EtwLogReader::CountWin32EventsByLevel(const std::wstring& wChannel, EtwEventLevelCounts& counts) {
                counts.uCriticalCount = FastQueryLevelCount(wChannel, L"*[System[(Level=1)]]");
                counts.uErrorCount = FastQueryLevelCount(wChannel, L"*[System[(Level=2)]]");
                counts.uWarningCount = FastQueryLevelCount(wChannel, L"*[System[(Level=3)]]");
                counts.uInfoCount = FastQueryLevelCount(wChannel, L"*[System[(Level=4 or Level=0)]]");
                counts.uVerboseCount = FastQueryLevelCount(wChannel, L"*[System[(Level=5)]]");
            }
#endif

            EtwEventLevelCounts EtwLogReader::GetChannelEventLevelCounts(const String& sChannelName) {
                EtwEventLevelCounts counts = { 0, 0, 0, 0, 0 };
                if (sChannelName.IsEmpty()) return counts;
                std::lock_guard<std::mutex> lock(s_mtxEtw);
#if defined(_WIN32) || defined(_WIN64)
                const char* pszRaw = sChannelName.GetRawString() ? sChannelName.GetRawString() : "";
                std::wstring wChannel = Utils::StringConvert::Utf8ToWChar(pszRaw);
                CountWin32EventsByLevel(wChannel, counts);
#endif
                return counts;
            }

            Collections::Generic::List<String> EtwLogReader::GetEventChannels() {
                std::lock_guard<std::mutex> lock(s_mtxEtw);
                Collections::Generic::List<String> lstChannels;
#if defined(_WIN32)
                EnumerateWin32Channels(lstChannels);
#endif
                for (const auto& ch : s_vRegisteredChannels) {
                    bool bExists = false;
                    for (int i = 0; i < lstChannels.GetCount(); i++) {
                        if (lstChannels[i].Equals(ch)) { bExists = true; break; }
                    }
                    if (!bExists) lstChannels.Add(ch);
                }
                return lstChannels;
            }

            Collections::Generic::List<EtwEvent> EtwLogReader::ReadEvents(const String& sChannelName) { return ReadEvents(sChannelName, 0, 0, false, EtwEventLevel::All); }
            Collections::Generic::List<EtwEvent> EtwLogReader::ReadEvents(const String& sChannelName, int iMaxEvents) { return ReadEvents(sChannelName, iMaxEvents, 0, false, EtwEventLevel::All); }
            Collections::Generic::List<EtwEvent> EtwLogReader::ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex) { return ReadEvents(sChannelName, iMaxEvents, iStartIndex, false, EtwEventLevel::All); }
            Collections::Generic::List<EtwEvent> EtwLogReader::ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection) { return ReadEvents(sChannelName, iMaxEvents, iStartIndex, bReverseDirection, EtwEventLevel::All); }

            static void FilterChannelEvents(const std::vector<EtwEvent>& events, EtwEventLevel level, int iStartIndex, int iMaxEvents, Collections::Generic::List<EtwEvent>& lstEvents) {
                int iSkipped = 0;
                for (const auto& evt : events) {
                    if (!MatchEventLevelFilter(evt, level)) continue;
                    if (iStartIndex > 0 && iSkipped++ < iStartIndex) continue;
                    lstEvents.Add(evt);
                    if (iMaxEvents > 0 && lstEvents.GetCount() >= iMaxEvents) break;
                }
            }

            Collections::Generic::List<EtwEvent> EtwLogReader::ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level) {
                if (sChannelName.IsEmpty()) throw ArgumentException("Channel name cannot be empty.");
                std::lock_guard<std::mutex> lock(s_mtxEtw);
                RegisterChannelIfNew(sChannelName);
                Collections::Generic::List<EtwEvent> lstEvents;
#if defined(_WIN32)
                ReadWin32EvtChannel(sChannelName, iMaxEvents, iStartIndex, bReverseDirection, level, lstEvents);
#endif
                auto it = s_mapChannelEvents.find(sChannelName);
                if (it != s_mapChannelEvents.end()) FilterChannelEvents(it->second, level, iStartIndex, iMaxEvents, lstEvents);
                return lstEvents;
            }

            void EtwLogReader::StartListening(const String& sChannelName, Action<const EtwEvent&> fnCallback) {
                if (sChannelName.IsEmpty()) throw ArgumentException("Channel name cannot be empty.");
                if (m_bListening) throw InvalidOperationException("Already listening to an event channel.");

                std::lock_guard<std::mutex> lock(s_mtxEtw);
                RegisterChannelIfNew(sChannelName);
                m_bListening = true;
                m_sListeningChannel = sChannelName;
                m_fnCallback = fnCallback;

#if defined(_WIN32)
                EVT_HANDLE hSub = SubscribeWin32Channel(sChannelName, &m_fnCallback);
                m_pSubscriptionHandle = (void*)hSub;
#endif
            }

            void EtwLogReader::StopListening() {
                std::lock_guard<std::mutex> lock(s_mtxEtw);
                if (!m_bListening) return;

#if defined(_WIN32)
                if (m_pSubscriptionHandle != nullptr) {
                    ::EvtClose((EVT_HANDLE)m_pSubscriptionHandle);
                    m_pSubscriptionHandle = nullptr;
                }
#endif

                m_bListening = false;
                m_sListeningChannel = "";
                m_fnCallback = nullptr;
            }

        }
    }
}
