#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/DateTimeOffset.h"
#include "System/Collections/Generic/List.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            enum class EventLogEntryType {
                Error = 1,
                Warning = 2,
                Information = 4,
                SuccessAudit = 8,
                FailureAudit = 16
            };

            class EventLogEntry : public Object {
            public:
                DOTNETDUPE_API EventLogEntry();
                DOTNETDUPE_API EventLogEntry(const String& sMessage, EventLogEntryType eType, int iInstanceId, const String& sSource, const DateTimeOffset& dtTimeGenerated);

                String GetMessage() const { return m_sMessage; }
                EventLogEntryType GetEntryType() const { return m_eEntryType; }
                int GetInstanceId() const { return m_iInstanceId; }
                String GetSource() const { return m_sSource; }
                DateTimeOffset GetTimeGenerated() const { return m_dtTimeGenerated; }

            private:
                String m_sMessage;
                EventLogEntryType m_eEntryType;
                int m_iInstanceId;
                String m_sSource;
                DateTimeOffset m_dtTimeGenerated;
            };

            class EventLog : public Object {
            public:
                DOTNETDUPE_API EventLog();
                DOTNETDUPE_API EventLog(const String& sLogName);
                DOTNETDUPE_API EventLog(const String& sLogName, const String& sMachineName);
                DOTNETDUPE_API EventLog(const String& sLogName, const String& sMachineName, const String& sSource);
                DOTNETDUPE_API virtual ~EventLog();

                String GetLog() const { return m_sLogName; }
                void SetLog(const String& sLogName) { m_sLogName = sLogName; }

                String GetMachineName() const { return m_sMachineName; }
                void SetMachineName(const String& sMachineName) { m_sMachineName = sMachineName; }

                String GetSource() const { return m_sSource; }
                void SetSource(const String& sSource) { m_sSource = sSource; }

                DOTNETDUPE_API Collections::Generic::List<EventLogEntry> GetEntries() const;

                DOTNETDUPE_API void WriteEntry(const String& sMessage);
                DOTNETDUPE_API void WriteEntry(const String& sMessage, EventLogEntryType eType);
                DOTNETDUPE_API void WriteEntry(const String& sMessage, EventLogEntryType eType, int iEventID);

                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage);
                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType);
                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID);

                DOTNETDUPE_API static bool SourceExists(const String& sSource);
                DOTNETDUPE_API static bool SourceExists(const String& sSource, const String& sMachineName);

                DOTNETDUPE_API static void CreateEventSource(const String& sSource, const String& sLogName);

                DOTNETDUPE_API static void Delete(const String& sLogName);
                DOTNETDUPE_API static void Delete(const String& sLogName, const String& sMachineName);

                DOTNETDUPE_API static void DeleteEventSource(const String& sSource);
                DOTNETDUPE_API static void DeleteEventSource(const String& sSource, const String& sMachineName);

                DOTNETDUPE_API static bool Exists(const String& sLogName);
                DOTNETDUPE_API static bool Exists(const String& sLogName, const String& sMachineName);

                DOTNETDUPE_API static Collections::Generic::List<EventLog> GetEventLogs();
                DOTNETDUPE_API static Collections::Generic::List<EventLog> GetEventLogs(const String& sMachineName);

                DOTNETDUPE_API void Clear();
                DOTNETDUPE_API void Close();

            private:
                String m_sLogName;
                String m_sMachineName;
                String m_sSource;

#if defined(_WIN32)
                static EventLogEntryType MapWin32EventType(WORD wType);
                static EventLogEntry ParseWin32Record(const PEVENTLOGRECORD pRec);
                static void ProcessWin32EventBuffer(BYTE* buffer, DWORD dwBytesRead, Collections::Generic::List<EventLogEntry>& lstEntries);
                static void ReadWin32EventLog(const String& sLogName, Collections::Generic::List<EventLogEntry>& lstEntries);
                static void WriteWin32EventLog(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID);
                static bool CreateWin32EventSource(const String& sSource, const String& sLogName);
                static bool Win32SourceExists(const String& sSource);
                static void DeleteWin32EventSource(const String& sSource);
#else
                static void WriteLinuxSyslog(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID);
                static EventLogEntry ParseSyslogLine(const String& line);
                static void ReadLinuxSyslogFile(const String& sFilePath, Collections::Generic::List<EventLogEntry>& lstEntries);
                static void ReadLinuxSyslog(Collections::Generic::List<EventLogEntry>& lstEntries);
#endif
                static void RecordInternalLogEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID);
                static void PurgeSourcesForLog(const String& sLogName);
            };

        }
    }
}
