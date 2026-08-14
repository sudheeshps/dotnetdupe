#include "pch.h"
#include "System/Diagnostics/EventLog.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/ComponentModel/Win32Exception.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Environment.h"
#include "System/TimeProvider.h"

#include <mutex>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
#include <windows.h>
#else
#include <syslog.h>
#include <sys/stat.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            struct EventLogStoreEntry {
                String sMessage;
                EventLogEntryType eType;
                int iInstanceId;
                String sSource;
                DateTimeOffset dtTimeGenerated;
            };

            static std::mutex s_mtxEventLog;
            static std::map<String, String> s_mapSourceToLog;
            static std::map<String, std::vector<EventLogStoreEntry>> s_mapLogEntries;

            EventLogEntry::EventLogEntry()
                : m_sMessage(""), m_eEntryType(EventLogEntryType::Information), m_iInstanceId(0), m_sSource(""), m_dtTimeGenerated(DateTimeOffset::Now()) {
            }

            EventLogEntry::EventLogEntry(const String& sMessage, EventLogEntryType eType, int iInstanceId, const String& sSource, const DateTimeOffset& dtTimeGenerated)
                : m_sMessage(sMessage), m_eEntryType(eType), m_iInstanceId(iInstanceId), m_sSource(sSource), m_dtTimeGenerated(dtTimeGenerated) {
            }

            EventLog::EventLog()
                : m_sLogName("Application"), m_sMachineName("."), m_sSource("") {
            }

            EventLog::EventLog(const String& sLogName)
                : m_sLogName(sLogName), m_sMachineName("."), m_sSource("") {
                if (m_sLogName.IsEmpty()) m_sLogName = "Application";
            }

            EventLog::EventLog(const String& sLogName, const String& sMachineName)
                : m_sLogName(sLogName), m_sMachineName(sMachineName), m_sSource("") {
                if (m_sLogName.IsEmpty()) m_sLogName = "Application";
                if (m_sMachineName.IsEmpty()) m_sMachineName = ".";
            }

            EventLog::EventLog(const String& sLogName, const String& sMachineName, const String& sSource)
                : m_sLogName(sLogName), m_sMachineName(sMachineName), m_sSource(sSource) {
                if (m_sLogName.IsEmpty()) m_sLogName = "Application";
                if (m_sMachineName.IsEmpty()) m_sMachineName = ".";
            }

            EventLog::~EventLog() {
            }

#if defined(_WIN32)
            EventLogEntryType EventLog::MapWin32EventType(WORD wType) {
                if (wType == EVENTLOG_ERROR_TYPE) return EventLogEntryType::Error;
                if (wType == EVENTLOG_WARNING_TYPE) return EventLogEntryType::Warning;
                if (wType == EVENTLOG_AUDIT_SUCCESS) return EventLogEntryType::SuccessAudit;
                if (wType == EVENTLOG_AUDIT_FAILURE) return EventLogEntryType::FailureAudit;
                return EventLogEntryType::Information;
            }

            EventLogEntry EventLog::ParseWin32Record(const PEVENTLOGRECORD pRec) {
                EventLogEntryType eType = MapWin32EventType(pRec->EventType);
                std::wstring wSrc((wchar_t*)((BYTE*)pRec + sizeof(EVENTLOGRECORD)));
                String sMsg = "";
                if (pRec->NumStrings > 0) {
                    std::wstring wMsg((wchar_t*)((BYTE*)pRec + pRec->StringOffset));
                    std::string sNarrowMsg;
                    for (wchar_t wc : wMsg) { sNarrowMsg += static_cast<char>(wc); }
                    sMsg = String(sNarrowMsg.c_str());
                }
                int64_t iTicks = ((int64_t)pRec->TimeGenerated + 62135596800LL) * 10000000LL;
                std::string sNarrowSrc;
                for (wchar_t wc : wSrc) { sNarrowSrc += static_cast<char>(wc); }
                return EventLogEntry(sMsg, eType, (int)pRec->EventID, String(sNarrowSrc.c_str()), DateTimeOffset(iTicks));
            }

            void EventLog::ProcessWin32EventBuffer(BYTE* buffer, DWORD dwBytesRead, Collections::Generic::List<EventLogEntry>& lstEntries) {
                DWORD dwOffset = 0;
                while (dwOffset < dwBytesRead) {
                    PEVENTLOGRECORD pRec = (PEVENTLOGRECORD)&buffer[dwOffset];
                    lstEntries.Add(ParseWin32Record(pRec));
                    dwOffset += pRec->Length;
                }
            }

            static WORD MapEventLogEntryTypeToWin32(EventLogEntryType eType) {
                if (eType == EventLogEntryType::Error) return EVENTLOG_ERROR_TYPE;
                if (eType == EventLogEntryType::Warning) return EVENTLOG_WARNING_TYPE;
                if (eType == EventLogEntryType::SuccessAudit) return EVENTLOG_AUDIT_SUCCESS;
                if (eType == EventLogEntryType::FailureAudit) return EVENTLOG_AUDIT_FAILURE;
                return EVENTLOG_INFORMATION_TYPE;
            }

            static void ValidateWin32ReportResult(BOOL bReported, DWORD dwErr, const String& sSource) {
                if (!bReported) {
                    if (dwErr == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied writing to EventLog for source: " + sSource);
                    }
                    throw ComponentModel::Win32Exception(dwErr, "Failed to report event to EventLog: " + sSource);
                }
            }

            void EventLog::ReadWin32EventLog(const String& sLogName, Collections::Generic::List<EventLogEntry>& lstEntries) {
                std::string sStdLogName(sLogName.GetRawString() ? sLogName.GetRawString() : "");
                std::wstring wLogName(sStdLogName.begin(), sStdLogName.end());
                HANDLE hEventLog = ::OpenEventLogW(NULL, wLogName.c_str());
                if (hEventLog == NULL) {
                    DWORD dwErr = ::GetLastError();
                    if (dwErr == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied opening EventLog: " + sLogName);
                    }
                    if (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND) {
                        throw ArgumentException("EventLog not found: " + sLogName);
                    }
                    throw ComponentModel::Win32Exception(dwErr, "Failed to open EventLog: " + sLogName);
                }

                DWORD dwBytesRead = 0, dwNeeded = 0;
                BYTE buffer[0x10000];
                while (::ReadEventLogW(hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ, 0, buffer, sizeof(buffer), &dwBytesRead, &dwNeeded)) {
                    ProcessWin32EventBuffer(buffer, dwBytesRead, lstEntries);
                }
                ::CloseEventLog(hEventLog);
            }

            void EventLog::WriteWin32EventLog(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID) {
                std::string sStdSource(sSource.GetRawString() ? sSource.GetRawString() : "");
                std::wstring wSource(sStdSource.begin(), sStdSource.end());
                HANDLE hEventLog = ::RegisterEventSourceW(NULL, wSource.c_str());
                if (hEventLog == NULL) {
                    DWORD dwErr = ::GetLastError();
                    if (dwErr == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied registering EventSource: " + sSource);
                    }
                    throw ComponentModel::Win32Exception(dwErr, "Failed to register EventSource: " + sSource);
                }

                WORD wType = MapEventLogEntryTypeToWin32(eType);
                std::string sStdMsg(sMessage.GetRawString() ? sMessage.GetRawString() : "");
                std::wstring wMsg(sStdMsg.begin(), sStdMsg.end());
                LPCWSTR pStrings[1] = { wMsg.c_str() };
                BOOL bReported = ::ReportEventW(hEventLog, wType, 0, (DWORD)iEventID, NULL, 1, 0, pStrings, NULL);
                DWORD dwErr = ::GetLastError();
                ::DeregisterEventSource(hEventLog);

                ValidateWin32ReportResult(bReported, dwErr, sSource);
            }

            bool EventLog::CreateWin32EventSource(const String& sSource, const String& sLogName) {
                std::string sStdLog(sLogName.GetRawString() ? sLogName.GetRawString() : "");
                std::string sStdSrc(sSource.GetRawString() ? sSource.GetRawString() : "");
                std::wstring wSubKey = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" +
                                      std::wstring(sStdLog.begin(), sStdLog.end()) + L"\\" +
                                      std::wstring(sStdSrc.begin(), sStdSrc.end());
                HKEY hKey = NULL;
                LONG lRes = ::RegCreateKeyExW(HKEY_LOCAL_MACHINE, wSubKey.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
                if (lRes == ERROR_SUCCESS) {
                    ::RegCloseKey(hKey);
                    return true;
                }
                if (lRes == ERROR_ACCESS_DENIED) {
                    throw UnauthorizedAccessException("Access denied creating EventLog source in registry under HKLM. Administrator privileges required.");
                }
                return false;
            }

            bool EventLog::Win32SourceExists(const String& sSource) {
                std::string sStdSrc(sSource.GetRawString() ? sSource.GetRawString() : "");
                const wchar_t* subKeys[] = { L"Application", L"System", L"Security" };
                for (const wchar_t* pLog : subKeys) {
                    std::wstring wSubKey = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" + std::wstring(pLog) + L"\\" + std::wstring(sStdSrc.begin(), sStdSrc.end());
                    HKEY hKey = NULL;
                    if (::RegOpenKeyExW(HKEY_LOCAL_MACHINE, wSubKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                        ::RegCloseKey(hKey);
                        return true;
                    }
                }
                return false;
            }

            void EventLog::DeleteWin32EventSource(const String& sSource) {
                const wchar_t* subKeys[] = { L"Application", L"System", L"Security" };
                for (const wchar_t* pLog : subKeys) {
                    std::wstring wSubKey = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" + std::wstring(pLog) + L"\\" + std::wstring(sSource.GetRawString(), sSource.GetRawString() + sSource.GetLength());
                    LONG lRes = ::RegDeleteKeyW(HKEY_LOCAL_MACHINE, wSubKey.c_str());
                    if (lRes == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Access denied deleting EventLog source from registry under HKLM. Administrator privileges required.");
                    }
                }
            }
#else
            void EventLog::WriteLinuxSyslog(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID) {
                int iPriority = LOG_INFO;
                const char* szLevelStr = "Information";
                if (eType == EventLogEntryType::Error) { iPriority = LOG_ERR; szLevelStr = "Error"; }
                else if (eType == EventLogEntryType::Warning) { iPriority = LOG_WARNING; szLevelStr = "Warning"; }
                else if (eType == EventLogEntryType::SuccessAudit) { iPriority = LOG_NOTICE; szLevelStr = "SuccessAudit"; }
                else if (eType == EventLogEntryType::FailureAudit) { iPriority = LOG_ALERT; szLevelStr = "FailureAudit"; }
                const char* pSrc = sSource.GetRawString() ? sSource.GetRawString() : "";
                const char* pMsg = sMessage.GetRawString() ? sMessage.GetRawString() : "";
                openlog(pSrc, LOG_PID | LOG_CONS, LOG_USER);
                syslog(iPriority, "[%s] [EventID %d] %s", szLevelStr, iEventID, pMsg);
                closelog();
            }

            EventLogEntry EventLog::ParseSyslogLine(const std::string& line) {
                EventLogEntryType eType = EventLogEntryType::Information;

                if (line.find("error") != std::string::npos || line.find("ERR") != std::string::npos || line.find("err") != std::string::npos || line.find("Error") != std::string::npos) {
                    eType = EventLogEntryType::Error;
                } else if (line.find("warn") != std::string::npos || line.find("WARN") != std::string::npos || line.find("Warn") != std::string::npos) {
                    eType = EventLogEntryType::Warning;
                }

                int iEventId = 0;
                size_t posId = line.find("[EventID ");
                if (posId != std::string::npos) {
                    iEventId = std::atoi(line.c_str() + posId + 9);
                }

                return EventLogEntry(String(line.c_str()), eType, iEventId, "syslog", DateTimeOffset::Now());
            }

            void EventLog::ReadLinuxSyslogFile(const std::string& sFilePath, Collections::Generic::List<EventLogEntry>& lstEntries) {
                std::ifstream infile(sFilePath);
                if (!infile.is_open()) return;

                std::string line;
                while (std::getline(infile, line)) {
                    if (line.empty()) continue;
                    lstEntries.Add(ParseSyslogLine(line));
                }
            }

            void EventLog::ReadLinuxSyslog(Collections::Generic::List<EventLogEntry>& lstEntries) {
                const char* syslogPaths[] = { "/var/log/syslog", "/var/log/messages" };
                for (const char* path : syslogPaths) {
                    struct stat st;
                    if (stat(path, &st) == 0) {
                        ReadLinuxSyslogFile(path, lstEntries);
                        break;
                    }
                }
            }
#endif

            Collections::Generic::List<EventLogEntry> EventLog::GetEntries() const {
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                Collections::Generic::List<EventLogEntry> lstEntries;
#if defined(_WIN32)
                ReadWin32EventLog(m_sLogName, lstEntries);
#else
                ReadLinuxSyslog(lstEntries);
#endif
                auto it = s_mapLogEntries.find(m_sLogName);
                if (it != s_mapLogEntries.end()) {
                    for (const auto& entry : it->second) {
                        lstEntries.Add(EventLogEntry(entry.sMessage, entry.eType, entry.iInstanceId, entry.sSource, entry.dtTimeGenerated));
                    }
                }
                return lstEntries;
            }

            void EventLog::WriteEntry(const String& sMessage) {
                WriteEntry(sMessage, EventLogEntryType::Information, 0);
            }

            void EventLog::WriteEntry(const String& sMessage, EventLogEntryType eType) {
                WriteEntry(sMessage, eType, 0);
            }

            void EventLog::WriteEntry(const String& sMessage, EventLogEntryType eType, int iEventID) {
                String sEffectiveSource = m_sSource.IsEmpty() ? m_sLogName : m_sSource;
                WriteEntry(sEffectiveSource, sMessage, eType, iEventID);
            }

            void EventLog::WriteEntry(const String& sSource, const String& sMessage) {
                WriteEntry(sSource, sMessage, EventLogEntryType::Information, 0);
            }

            void EventLog::WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType) {
                WriteEntry(sSource, sMessage, eType, 0);
            }

            void EventLog::RecordInternalLogEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID) {
                String sTargetLog = "Application";
                auto itSource = s_mapSourceToLog.find(sSource);
                if (itSource != s_mapSourceToLog.end()) {
                    sTargetLog = itSource->second;
                } else {
                    s_mapSourceToLog[sSource] = sTargetLog;
                }
                EventLogStoreEntry entry{ sMessage, eType, iEventID, sSource, DateTimeOffset::Now() };
                s_mapLogEntries[sTargetLog].push_back(entry);
            }

            void EventLog::WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID) {
                if (sSource.IsEmpty()) {
                    throw ArgumentException("Source cannot be empty when writing to event log.");
                }
#if defined(_WIN32)
                WriteWin32EventLog(sSource, sMessage, eType, iEventID);
#else
                WriteLinuxSyslog(sSource, sMessage, eType, iEventID);
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                RecordInternalLogEntry(sSource, sMessage, eType, iEventID);
            }

            bool EventLog::SourceExists(const String& sSource) {
                return SourceExists(sSource, ".");
            }

            bool EventLog::SourceExists(const String& sSource, const String& sMachineName) {
                if (sSource.IsEmpty()) return false;
#if defined(_WIN32)
                if (Win32SourceExists(sSource)) return true;
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                return s_mapSourceToLog.find(sSource) != s_mapSourceToLog.end();
            }

            void EventLog::CreateEventSource(const String& sSource, const String& sLogName) {
                if (sSource.IsEmpty()) throw ArgumentException("Source cannot be empty.");
                String sEffectiveLog = sLogName.IsEmpty() ? String("Application") : sLogName;
#if defined(_WIN32)
                try {
                    CreateWin32EventSource(sSource, sEffectiveLog);
                } catch (const UnauthorizedAccessException&) {
                    // Fallback to in-memory store in non-elevated environments
                }
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                auto it = s_mapSourceToLog.find(sSource);
                if (it != s_mapSourceToLog.end() && it->second != sEffectiveLog) {
                    throw ArgumentException("Source already exists registered to another log.");
                }
                s_mapSourceToLog[sSource] = sEffectiveLog;
                if (s_mapLogEntries.find(sEffectiveLog) == s_mapLogEntries.end()) {
                    s_mapLogEntries[sEffectiveLog] = std::vector<EventLogStoreEntry>();
                }
            }

            void EventLog::Delete(const String& sLogName) {
                Delete(sLogName, ".");
            }

            void EventLog::PurgeSourcesForLog(const String& sLogName) {
                std::vector<String> vSourcesToRemove;
                for (const auto& pair : s_mapSourceToLog) {
                    if (pair.second == sLogName) vSourcesToRemove.push_back(pair.first);
                }
                for (const auto& sSource : vSourcesToRemove) {
                    s_mapSourceToLog.erase(sSource);
                }
            }

            void EventLog::Delete(const String& sLogName, const String& sMachineName) {
                if (sLogName.IsEmpty()) throw ArgumentException("Log name cannot be empty.");
#if defined(_WIN32)
                std::string sStdLog(sLogName.GetRawString() ? sLogName.GetRawString() : "");
                std::wstring wLogKey = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" + std::wstring(sStdLog.begin(), sStdLog.end());
                ::RegDeleteKeyW(HKEY_LOCAL_MACHINE, wLogKey.c_str());
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                s_mapLogEntries.erase(sLogName);
                PurgeSourcesForLog(sLogName);
            }

            void EventLog::DeleteEventSource(const String& sSource) {
                DeleteEventSource(sSource, ".");
            }

            void EventLog::DeleteEventSource(const String& sSource, const String& sMachineName) {
                if (sSource.IsEmpty()) throw ArgumentException("Source cannot be empty.");
#if defined(_WIN32)
                try {
                    DeleteWin32EventSource(sSource);
                } catch (const UnauthorizedAccessException&) {
                    // Fallback to in-memory store in non-elevated environments
                }
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                if (s_mapSourceToLog.find(sSource) == s_mapSourceToLog.end()) {
                    throw ArgumentException("The event source does not exist.");
                }
                s_mapSourceToLog.erase(sSource);
            }

            bool EventLog::Exists(const String& sLogName) {
                return Exists(sLogName, ".");
            }

            bool EventLog::Exists(const String& sLogName, const String& sMachineName) {
                if (sLogName.IsEmpty()) return false;
#if defined(_WIN32)
                std::string sStdLog(sLogName.GetRawString() ? sLogName.GetRawString() : "");
                std::wstring wSubKey = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" + std::wstring(sStdLog.begin(), sStdLog.end());
                HKEY hKey = NULL;
                if (::RegOpenKeyExW(HKEY_LOCAL_MACHINE, wSubKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                    ::RegCloseKey(hKey);
                    return true;
                }
#else
                if (sLogName == "Application" || sLogName == "System") return true;
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                return s_mapLogEntries.find(sLogName) != s_mapLogEntries.end();
            }

            Collections::Generic::List<EventLog> EventLog::GetEventLogs() {
                return GetEventLogs(".");
            }

            Collections::Generic::List<EventLog> EventLog::GetEventLogs(const String& sMachineName) {
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                Collections::Generic::List<EventLog> lstLogs;
                for (const auto& pair : s_mapLogEntries) {
                    lstLogs.Add(EventLog(pair.first, sMachineName));
                }
                return lstLogs;
            }

            void EventLog::Clear() {
#if defined(_WIN32)
                std::string sStdLog(m_sLogName.GetRawString() ? m_sLogName.GetRawString() : "");
                std::wstring wLog(sStdLog.begin(), sStdLog.end());
                HANDLE hLog = ::OpenEventLogW(NULL, wLog.c_str());
                if (hLog != NULL) {
                    ::ClearEventLogW(hLog, NULL);
                    ::CloseEventLog(hLog);
                }
#endif
                std::lock_guard<std::mutex> lock(s_mtxEventLog);
                s_mapLogEntries[m_sLogName].clear();
            }

            void EventLog::Close() {
            }

        }
    }
}
