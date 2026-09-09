#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Action.h"
#include "System/Collections/Generic/List.h"
#include "System/Diagnostics/EventLog.h"

#if defined(_WIN32)
#include <windows.h>
#include <winevt.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            /// \struct EtwEvent
            /// \brief Represents an individual Event Tracing for Windows (ETW) event record.
            struct EtwEvent {
                String sChannelName;            ///< Name of the event log channel (e.g. "Application", "System").
                String sMessage;                ///< Formatted textual description of the event.
                int iEventId;                   ///< Numeric identifier for the event type.
                int iLevel;                     ///< Severity level of the event.
                String sProviderName;           ///< Name or GUID of the publishing event provider.
                DateTimeOffset dtTimeCreated;   ///< Precise timestamp when the event was generated.
                String sRawXml;                 ///< Raw XML representation of the ETW payload.
            };

            /// \enum EtwEventLevel
            /// \brief Filter levels corresponding to standard Windows ETW severity classifications.
            enum class EtwEventLevel {
                All = 0,        ///< All events regardless of level.
                Critical = 1,   ///< Abnormal exit or severe failure requiring immediate intervention.
                Error = 2,      ///< Significant problem that indicates a runtime failure.
                Warning = 3,    ///< Non-critical condition that indicates potential future problems.
                Info = 4,       ///< Normal operational informational events.
                Verbose = 5     ///< Detailed developer or diagnostic trace information.
            };

            /// \struct EtwEventLevelCounts
            /// \brief Aggregate counts of events partitioned by severity level within a channel.
            struct EtwEventLevelCounts {
                unsigned long long uCriticalCount;  ///< Number of Critical level events.
                unsigned long long uErrorCount;     ///< Number of Error level events.
                unsigned long long uWarningCount;   ///< Number of Warning level events.
                unsigned long long uInfoCount;      ///< Number of Information level events.
                unsigned long long uVerboseCount;   ///< Number of Verbose level events.
            };

            /// \class EtwLogReader
            /// \brief High-performance reader and real-time subscriber for Windows Event Log / ETW channels.
            ///
            /// Interfaces with the Windows Event Log API (`winevt.h` / `wevtapi.lib`) to query, paginate,
            /// count, and asynchronously subscribe to structured event channels.
            ///
            /// \note Thread Safety: Channel querying methods are thread-safe and guarded by internal mutexes.
            class EtwLogReader : public Object {
            public:
                /// \brief Initializes a new instance of EtwLogReader.
                DOTNETDUPE_API EtwLogReader();

                /// \brief Destructor ensuring active asynchronous event subscriptions are terminated.
                DOTNETDUPE_API virtual ~EtwLogReader();

                /// \brief Enumerates all registered ETW and Windows Event Log channel paths.
                /// \return List of channel path strings.
                DOTNETDUPE_API static Collections::Generic::List<String> GetEventChannels();

                /// \brief Retrieves total number of recorded events in a channel.
                /// \param sChannelName The target channel path.
                /// \return The total record count.
                DOTNETDUPE_API static unsigned long long GetChannelEventCount(const String& sChannelName);

                /// \brief Aggregates the count of events in a channel broken down by severity level.
                /// \param sChannelName The target channel path.
                /// \return EtwEventLevelCounts structure containing breakdown.
                DOTNETDUPE_API static EtwEventLevelCounts GetChannelEventLevelCounts(const String& sChannelName);

                /// \brief Reads all available events from the specified channel.
                /// \param sChannelName The channel name.
                /// \return List of EtwEvent records.
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName);

                /// \brief Reads up to a maximum number of events from the specified channel.
                /// \param sChannelName The channel name.
                /// \param iMaxEvents Maximum records to retrieve (0 for all).
                /// \return List of EtwEvent records.
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents);

                /// \brief Reads events from a channel starting from a specific offset.
                /// \param sChannelName The channel name.
                /// \param iMaxEvents Maximum records to retrieve.
                /// \param iStartIndex Zero-based start index or record offset.
                /// \return List of EtwEvent records.
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex);

                /// \brief Reads events with direction control (forward or reverse chronological order).
                /// \param sChannelName The channel name.
                /// \param iMaxEvents Maximum records to retrieve.
                /// \param iStartIndex Zero-based start index.
                /// \param bReverseDirection True to read newest-to-oldest; false for oldest-to-newest.
                /// \return List of EtwEvent records.
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection);

                /// \brief Reads events matching a specific severity level filter.
                /// \param sChannelName The channel name.
                /// \param iMaxEvents Maximum records to retrieve.
                /// \param iStartIndex Zero-based start index.
                /// \param bReverseDirection Direction flag.
                /// \param level Target severity level filter.
                /// \return List of filtered EtwEvent records.
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level);

                /// \brief Initiates an asynchronous real-time subscription to events on the specified channel.
                /// \param sChannelName Target channel name.
                /// \param fnCallback Callback invoked on each newly arrived event.
                /// \throws DotNetDupe::System::InvalidOperationException If already listening.
                /// \throws DotNetDupe::System::UnauthorizedAccessException If access is denied.
                DOTNETDUPE_API void StartListening(const String& sChannelName, Action<const EtwEvent&> fnCallback);

                /// \brief Terminates active real-time event listening and closes subscription handles.
                DOTNETDUPE_API void StopListening();

                /// \brief Indicates whether an active subscription is running.
                /// \return True if currently listening; otherwise false.
                bool IsListening() const { return m_bListening; }

                /// \brief Retrieves the name of the channel currently being listened to.
                /// \return The active channel name string.
                String GetListeningChannel() const { return m_sListeningChannel; }

            private:
                bool m_bListening;
                String m_sListeningChannel;
                void* m_pSubscriptionHandle;
                Action<const EtwEvent&> m_fnCallback;

                static void RegisterChannelIfNew(const String& sChannelName);
#if defined(_WIN32)
                static DWORD WINAPI Win32EvtSubscribeCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pUserContext, EVT_HANDLE hEvent);
                static EVT_HANDLE SubscribeWin32Channel(const String& sChannelName, Action<const EtwEvent&>* pCallback);
                static void EnumerateWin32Channels(Collections::Generic::List<String>& lstChannels);
                static void FormatEtwEventXml(EVT_HANDLE hEvt, EtwEvent& evt);
                static void FormatEtwEventMessage(EVT_HANDLE hEvt, EtwEvent& evt);
                static EtwEvent ProcessSingleEtwEvent(EVT_HANDLE hContext, EVT_HANDLE hEvt, const String& sChannelName);
                static bool IterateEvtBatch(EVT_HANDLE hContext, EVT_HANDLE* arrEvents, DWORD dwReturned, const String& sChannelName, int iMaxEvents, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents);
                static void IterateEvtResults(EVT_HANDLE hContext, EVT_HANDLE hResults, const String& sChannelName, int iMaxEvents, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents);
                static void ReadWin32EvtChannel(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, EtwEventLevel level, Collections::Generic::List<EtwEvent>& lstEvents);
                static unsigned long long FastQueryLevelCount(const std::wstring& wChannel, const wchar_t* pwszFilter);
                static void CountWin32EventsByLevel(const std::wstring& wChannel, EtwEventLevelCounts& counts);
#endif
            };

        }
    }
}
