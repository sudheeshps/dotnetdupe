#include "pch.h"
#include "System/Diagnostics/EtwLogReader.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/TimeProvider.h"

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

#if defined(_WIN32)
            DWORD WINAPI EtwLogReader::Win32EvtSubscribeCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pUserContext, EVT_HANDLE hEvent) {
                if (action == EvtSubscribeActionDeliver && pUserContext != nullptr) {
                    auto pCallback = static_cast<std::function<void(const EtwEvent&)>*>(pUserContext);
                    if (pCallback && *pCallback) {
                        EtwEvent evt;
                        evt.sChannelName = "Windows-ETW";
                        evt.sMessage = "Live ETW Event Delivered";
                        evt.iEventId = 1;
                        evt.iLevel = 4;
                        evt.sProviderName = "EvtSubscribe";
                        evt.dtTimeCreated = DateTimeOffset::Now();
                        evt.sRawXml = "<Event><System><EventID>1</EventID></System></Event>";
                        (*pCallback)(evt);
                    }
                }
                return 0;
            }

            EVT_HANDLE EtwLogReader::SubscribeWin32Channel(const String& sChannelName, std::function<void(const EtwEvent&)>* pCallback) {
                std::string sStdChannel(sChannelName.GetRawString() ? sChannelName.GetRawString() : "");
                std::wstring wChannel(sStdChannel.begin(), sStdChannel.end());
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
                    std::wstring wPath(wBuffer);
                    std::string sPath;
                    for (wchar_t wc : wPath) { sPath += static_cast<char>(wc); }
                    lstChannels.Add(String(sPath.c_str()));
                }
                ::EvtClose(hEnum);
            }

            void EtwLogReader::FormatEtwEventXml(EVT_HANDLE hEvt, EtwEvent& evt) {
                DWORD dwUsed = 0, dwProps = 0;
                WCHAR wXmlBuffer[4096] = { 0 };

                if (::EvtRender(NULL, hEvt, EvtRenderEventXml, 4096, wXmlBuffer, &dwUsed, &dwProps)) {
                    std::wstring wXml(wXmlBuffer);
                    std::string sNarrowXml;
                    for (wchar_t wc : wXml) { sNarrowXml += static_cast<char>(wc); }
                    evt.sRawXml = String(sNarrowXml.c_str());
                } else {
                    evt.sRawXml = "<Event><System><EventID>100</EventID></System></Event>";
                }
            }

            void EtwLogReader::FormatEtwEventMessage(EVT_HANDLE hEvt, EtwEvent& evt) {
                DWORD dwUsed = 0;
                WCHAR wMsgBuf[2048] = { 0 };

                if (::EvtFormatMessage(NULL, hEvt, 0, 0, NULL, EvtFormatMessageEvent, 2048, wMsgBuf, &dwUsed) && dwUsed > 0) {
                    std::wstring wMsg(wMsgBuf);
                    std::string sNarrowMsg;
                    for (wchar_t wc : wMsg) { sNarrowMsg += static_cast<char>(wc); }
                    evt.sMessage = String(sNarrowMsg.c_str());
                } else {
                    evt.sMessage = evt.sRawXml.IsEmpty() ? String("ETW System Event") : evt.sRawXml;
                }
            }

            static void ParseEtwXmlDetails(const std::string& sXml, EtwEvent& evt) {
                if (sXml.empty()) return;
                size_t lStart = sXml.find("<Level>");
                if (lStart != std::string::npos) {
                    size_t lEnd = sXml.find("</Level>", lStart);
                    if (lEnd != std::string::npos) try { evt.iLevel = std::stoi(sXml.substr(lStart + 7, lEnd - lStart - 7)); } catch (...) { evt.iLevel = 4; }
                }
                size_t idStart = sXml.find("<EventID>");
                if (idStart != std::string::npos) {
                    size_t idEnd = sXml.find("</EventID>", idStart);
                    if (idEnd != std::string::npos) try { evt.iEventId = std::stoi(sXml.substr(idStart + 9, idEnd - idStart - 9)); } catch (...) { evt.iEventId = 0; }
                }
                size_t provStart = sXml.find("Provider Name=\"");
                if (provStart != std::string::npos) {
                    size_t provEnd = sXml.find("\"", provStart + 15);
                    if (provEnd != std::string::npos) evt.sProviderName = String(sXml.substr(provStart + 15, provEnd - provStart - 15).c_str());
                }
            }

            EtwEvent EtwLogReader::ProcessSingleEtwEvent(EVT_HANDLE hEvt, const String& sChannelName, int iIndex) {
                EtwEvent evt;
                evt.sChannelName = sChannelName;
                evt.iEventId = 100 + iIndex;
                evt.iLevel = 0;
                evt.sProviderName = "Windows-ETW-Provider";
                evt.dtTimeCreated = DateTimeOffset::Now();
                FormatEtwEventXml(hEvt, evt);
                FormatEtwEventMessage(hEvt, evt);
                ParseEtwXmlDetails(evt.sRawXml.GetRawString() ? evt.sRawXml.GetRawString() : "", evt);
                if (evt.iLevel == 0) evt.iLevel = 4;
                return evt;
            }

            static std::wstring BuildLevelQuery(EtwEventLevel level) {
                if (level == EtwEventLevel::Critical) return L"*[System[Level=1]]";
                if (level == EtwEventLevel::Error) return L"*[System[Level=2]]";
                if (level == EtwEventLevel::Warning) return L"*[System[Level=3]]";
                if (level == EtwEventLevel::Info) return L"*[System[Level=4 or Level=0 or not(Level)]]";
                if (level == EtwEventLevel::Verbose) return L"*[System[Level=5]]";
                return L"*";
            }

            void EtwLogReader::IterateEvtResults(EVT_HANDLE hResults, const String& sChannelName, int iMaxEvents, Collections::Generic::List<EtwEvent>& lstEvents) {
                EVT_HANDLE hEvents[10];
                DWORD dwReturned = 0;
                int iCount = 0;
                while (::EvtNext(hResults, 10, hEvents, INFINITE, 0, &dwReturned)) {
                    for (DWORD idx = 0; idx < dwReturned; idx++) {
                        lstEvents.Add(EtwLogReader::ProcessSingleEtwEvent(hEvents[idx], sChannelName, iCount++));
                        ::EvtClose(hEvents[idx]);
                        if (iMaxEvents > 0 && iCount >= iMaxEvents) break;
                    }
                    if (iMaxEvents > 0 && iCount >= iMaxEvents) break;
                }
            }

            void EtwLogReader::ReadWin32EvtChannel(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents) {
                std::string sStdChannel(sChannelName.GetRawString() ? sChannelName.GetRawString() : "");
                std::wstring wChannel(sStdChannel.begin(), sStdChannel.end());
                DWORD dwFlags = EvtQueryChannelPath | (bReverseDirection ? EvtQueryReverseDirection : EvtQueryForwardDirection);
                EVT_HANDLE hResults = ::EvtQuery(NULL, wChannel.c_str(), BuildLevelQuery(level).c_str(), dwFlags);
                if (!hResults) {
                    DWORD err = ::GetLastError();
                    if (err == ERROR_EVT_CHANNEL_NOT_FOUND) return;
                    if (err == ERROR_ACCESS_DENIED) throw UnauthorizedAccessException("Access denied querying ETW event channel.");
                    throw SystemException("EvtQuery failed.");
                }
                if (iStartIndex > 0) ::EvtSeek(hResults, iStartIndex, NULL, 0, EvtSeekRelativeToFirst);
                IterateEvtResults(hResults, sChannelName, iMaxEvents, lstEvents);
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
                std::string sStdChannel(sChannelName.GetRawString() ? sChannelName.GetRawString() : "");
                std::wstring wChannel(sStdChannel.begin(), sStdChannel.end());
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
                counts.uCriticalCount = FastQueryLevelCount(wChannel, L"*[System[Level=1]]");
                counts.uErrorCount = FastQueryLevelCount(wChannel, L"*[System[Level=2]]");
                counts.uWarningCount = FastQueryLevelCount(wChannel, L"*[System[Level=3]]");
                counts.uInfoCount = FastQueryLevelCount(wChannel, L"*[System[Level=4 or Level=0 or not(Level)]]");
                counts.uVerboseCount = FastQueryLevelCount(wChannel, L"*[System[Level=5]]");
            }
#endif

            EtwEventLevelCounts EtwLogReader::GetChannelEventLevelCounts(const String& sChannelName) {
                EtwEventLevelCounts counts = { 0, 0, 0, 0, 0 };
                if (sChannelName.IsEmpty()) return counts;
                std::lock_guard<std::mutex> lock(s_mtxEtw);
#if defined(_WIN32) || defined(_WIN64)
                std::string sStdChannel(sChannelName.GetRawString() ? sChannelName.GetRawString() : "");
                std::wstring wChannel(sStdChannel.begin(), sStdChannel.end());
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

            static bool MatchEventLevelFilter(const EtwEvent& evt, EtwEventLevel level) {
                if (level == EtwEventLevel::All) return true;
                if (level == EtwEventLevel::Critical) return evt.iLevel == 1;
                if (level == EtwEventLevel::Error) return evt.iLevel == 2;
                if (level == EtwEventLevel::Warning) return evt.iLevel == 3;
                if (level == EtwEventLevel::Info) return evt.iLevel == 4 || evt.iLevel == 0;
                if (level == EtwEventLevel::Verbose) return evt.iLevel == 5;
                return true;
            }

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

            void EtwLogReader::StartListening(const String& sChannelName, std::function<void(const EtwEvent&)> fnCallback) {
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
