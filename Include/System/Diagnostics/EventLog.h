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

            /// \enum EventLogEntryType
            /// \brief Defines the event type of an event log entry.
            ///
            /// Modeled after .NET System.Diagnostics.EventLogEntryType.
            enum class EventLogEntryType {
                Error = 1,          ///< An error event indicating significant problem.
                Warning = 2,        ///< A warning event indicating a potential problem.
                Information = 4,    ///< An informational event representing successful milestones.
                SuccessAudit = 8,   ///< An audit event tracking successful security access.
                FailureAudit = 16   ///< An audit event tracking failed security access.
            };

            /// \class EventLogEntry
            /// \brief Encapsulates a single record in the event log.
            ///
            /// Modeled after .NET System.Diagnostics.EventLogEntry.
            class EventLogEntry : public Object {
            public:
                /// \brief Default constructor initializing an empty entry.
                DOTNETDUPE_API EventLogEntry();

                /// \brief Constructs an EventLogEntry with specified properties.
                /// \param sMessage The text message of the event.
                /// \param eType The event severity classification.
                /// \param iInstanceId The resource identifier or event ID.
                /// \param sSource The application source that created the event.
                /// \param dtTimeGenerated Timestamp when the event was generated.
                DOTNETDUPE_API EventLogEntry(const String& sMessage, EventLogEntryType eType, int iInstanceId, const String& sSource, const DateTimeOffset& dtTimeGenerated);

                /// \brief Gets the message associated with the event.
                /// \return The event text message.
                String GetMessage() const { return m_sMessage; }

                /// \brief Gets the event type of the entry.
                /// \return The EventLogEntryType classification.
                EventLogEntryType GetEntryType() const { return m_eEntryType; }

                /// \brief Gets the resource identifier / event ID.
                /// \return The 32-bit instance identifier.
                int GetInstanceId() const { return m_iInstanceId; }

                /// \brief Gets the name of the software that registered the event.
                /// \return The source name string.
                String GetSource() const { return m_sSource; }

                /// \brief Gets the local time at which this event was generated.
                /// \return The generation timestamp.
                DateTimeOffset GetTimeGenerated() const { return m_dtTimeGenerated; }

            private:
                String m_sMessage;
                EventLogEntryType m_eEntryType;
                int m_iInstanceId;
                String m_sSource;
                DateTimeOffset m_dtTimeGenerated;
            };

            /// \class EventLog
            /// \brief Provides interaction with Windows event logs and Linux syslog services.
            ///
            /// Modeled after .NET System.Diagnostics.EventLog. Supports reading, writing, creating,
            /// and clearing system event logs across platforms. On Windows, reads via `OpenEventLogW`
            /// and writes via `ReportEventW`; on Linux, interfaces with POSIX `syslog` (RFC 5424).
            ///
            /// \note Thread Safety: Static methods and internal registry access are thread-safe.
            class EventLog : public Object {
            public:
                /// \brief Initializes a new instance of the EventLog class targeting the Application log.
                DOTNETDUPE_API EventLog();

                /// \brief Initializes an instance targeting the specified log on the local computer.
                /// \param sLogName The name of the log (e.g. "Application", "System").
                DOTNETDUPE_API EventLog(const String& sLogName);

                /// \brief Initializes an instance targeting the specified log and machine.
                /// \param sLogName The log name.
                /// \param sMachineName The computer name ("." for local).
                DOTNETDUPE_API EventLog(const String& sLogName, const String& sMachineName);

                /// \brief Initializes an instance targeting the specified log, machine, and source.
                /// \param sLogName The log name.
                /// \param sMachineName The computer name ("." for local).
                /// \param sSource The application event source name.
                DOTNETDUPE_API EventLog(const String& sLogName, const String& sMachineName, const String& sSource);

                /// \brief Virtual destructor.
                DOTNETDUPE_API virtual ~EventLog();

                /// \brief Gets the name of the log to read from or write to.
                /// \return The log name string.
                String GetLog() const { return m_sLogName; }

                /// \brief Sets the name of the log to read from or write to.
                /// \param sLogName The log name string.
                void SetLog(const String& sLogName) { m_sLogName = sLogName; }

                /// \brief Gets the name of the computer on which the event log resides.
                /// \return The machine name.
                String GetMachineName() const { return m_sMachineName; }

                /// \brief Sets the name of the computer on which the event log resides.
                /// \param sMachineName The computer name.
                void SetMachineName(const String& sMachineName) { m_sMachineName = sMachineName; }

                /// \brief Gets the source name to register and use when writing to the event log.
                /// \return The source name.
                String GetSource() const { return m_sSource; }

                /// \brief Sets the source name to register and use when writing to the event log.
                /// \param sSource The source name.
                void SetSource(const String& sSource) { m_sSource = sSource; }

                /// \brief Gets the contents of the event log as a list of entries.
                /// \return List of EventLogEntry objects.
                DOTNETDUPE_API Collections::Generic::List<EventLogEntry> GetEntries() const;

                /// \brief Writes an information entry with the specified message to the event log.
                /// \param sMessage The text string to write.
                DOTNETDUPE_API void WriteEntry(const String& sMessage);

                /// \brief Writes an entry with the specified message and severity type.
                /// \param sMessage The text string to write.
                /// \param eType The severity classification.
                DOTNETDUPE_API void WriteEntry(const String& sMessage, EventLogEntryType eType);

                /// \brief Writes an entry with specified message, severity, and event identifier.
                /// \param sMessage The text string to write.
                /// \param eType The severity classification.
                /// \param iEventID The application-specific event identifier.
                DOTNETDUPE_API void WriteEntry(const String& sMessage, EventLogEntryType eType, int iEventID);

                /// \brief Writes an information entry to an event log using the specified source.
                /// \param sSource The source by which the application is registered.
                /// \param sMessage The text message to write.
                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage);

                /// \brief Writes an entry with specified source, message, and entry type.
                /// \param sSource The registered source name.
                /// \param sMessage The text message.
                /// \param eType The severity classification.
                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType);

                /// \brief Writes an entry with specified source, message, type, and event identifier.
                /// \param sSource The registered source name.
                /// \param sMessage The text message.
                /// \param eType The severity classification.
                /// \param iEventID The event identifier.
                DOTNETDUPE_API static void WriteEntry(const String& sSource, const String& sMessage, EventLogEntryType eType, int iEventID);

                /// \brief Determines whether the specified event source is registered on the local computer.
                /// \param sSource The name of the event source.
                /// \return True if source exists; otherwise false.
                DOTNETDUPE_API static bool SourceExists(const String& sSource);

                /// \brief Determines whether the specified event source is registered on the given computer.
                /// \param sSource The name of the event source.
                /// \param sMachineName The machine name to check.
                /// \return True if source exists; otherwise false.
                DOTNETDUPE_API static bool SourceExists(const String& sSource, const String& sMachineName);

                /// \brief Establishes the specified source name as a valid event source for writing to a log.
                /// \param sSource The source name to register.
                /// \param sLogName The target log name.
                DOTNETDUPE_API static void CreateEventSource(const String& sSource, const String& sLogName);

                /// \brief Removes an event log from the local computer.
                /// \param sLogName The name of the log to delete.
                DOTNETDUPE_API static void Delete(const String& sLogName);

                /// \brief Removes an event log from the specified computer.
                /// \param sLogName The name of the log to delete.
                /// \param sMachineName The machine name.
                DOTNETDUPE_API static void Delete(const String& sLogName, const String& sMachineName);

                /// \brief Removes an event source registration from the local computer.
                /// \param sSource The source name to remove.
                DOTNETDUPE_API static void DeleteEventSource(const String& sSource);

                /// \brief Removes an event source registration from the specified computer.
                /// \param sSource The source name to remove.
                /// \param sMachineName The machine name.
                DOTNETDUPE_API static void DeleteEventSource(const String& sSource, const String& sMachineName);

                /// \brief Determines whether the log exists on the local computer.
                /// \param sLogName The log name to check.
                /// \return True if the log exists; otherwise false.
                DOTNETDUPE_API static bool Exists(const String& sLogName);

                /// \brief Determines whether the log exists on the specified computer.
                /// \param sLogName The log name.
                /// \param sMachineName The computer name.
                /// \return True if the log exists; otherwise false.
                DOTNETDUPE_API static bool Exists(const String& sLogName, const String& sMachineName);

                /// \brief Searches for all event logs on the local computer.
                /// \return List of EventLog instances.
                DOTNETDUPE_API static Collections::Generic::List<EventLog> GetEventLogs();

                /// \brief Searches for all event logs on the specified computer.
                /// \param sMachineName The computer name.
                /// \return List of EventLog instances.
                DOTNETDUPE_API static Collections::Generic::List<EventLog> GetEventLogs(const String& sMachineName);

                /// \brief Removes all entries from the event log.
                DOTNETDUPE_API void Clear();

                /// \brief Closes the event log and releases read/write handles.
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
