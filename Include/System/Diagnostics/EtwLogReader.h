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

            struct EtwEvent {
                String sChannelName;
                String sMessage;
                int iEventId;
                int iLevel;
                String sProviderName;
                DateTimeOffset dtTimeCreated;
                String sRawXml;
            };

            struct EtwEventLevelCounts {
                unsigned long long uCriticalCount;
                unsigned long long uErrorCount;
                unsigned long long uWarningCount;
                unsigned long long uInfoCount;
                unsigned long long uVerboseCount;
            };

            class EtwLogReader : public Object {
            public:
                DOTNETDUPE_API EtwLogReader();
                DOTNETDUPE_API virtual ~EtwLogReader();

                DOTNETDUPE_API static Collections::Generic::List<String> GetEventChannels();
                DOTNETDUPE_API static unsigned long long GetChannelEventCount(const String& sChannelName);
                DOTNETDUPE_API static EtwEventLevelCounts GetChannelEventLevelCounts(const String& sChannelName);

                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName);
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents);
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex);
                DOTNETDUPE_API static Collections::Generic::List<EtwEvent> ReadEvents(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection);

                DOTNETDUPE_API void StartListening(const String& sChannelName, std::function<void(const EtwEvent&)> fnCallback);
                DOTNETDUPE_API void StopListening();

                bool IsListening() const { return m_bListening; }
                String GetListeningChannel() const { return m_sListeningChannel; }

            private:
                bool m_bListening;
                String m_sListeningChannel;
                void* m_pSubscriptionHandle;
                std::function<void(const EtwEvent&)> m_fnCallback;

                static void RegisterChannelIfNew(const String& sChannelName);
#if defined(_WIN32)
                static DWORD WINAPI Win32EvtSubscribeCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pUserContext, EVT_HANDLE hEvent);
                static EVT_HANDLE SubscribeWin32Channel(const String& sChannelName, std::function<void(const EtwEvent&)>* pCallback);
                static void EnumerateWin32Channels(Collections::Generic::List<String>& lstChannels);
                static void FormatEtwEventXml(EVT_HANDLE hEvt, EtwEvent& evt);
                static void FormatEtwEventMessage(EVT_HANDLE hEvt, EtwEvent& evt);
                static EtwEvent ProcessSingleEtwEvent(EVT_HANDLE hEvt, const String& sChannelName, int iIndex);
                static void ReadWin32EvtChannel(const String& sChannelName, int iMaxEvents, int iStartIndex, bool bReverseDirection, Collections::Generic::List<EtwEvent>& lstEvents);
                static unsigned long long FastQueryLevelCount(const std::wstring& wChannel, const wchar_t* pwszFilter);
                static void CountWin32EventsByLevel(const std::wstring& wChannel, EtwEventLevelCounts& counts);
#endif
            };

        }
    }
}
