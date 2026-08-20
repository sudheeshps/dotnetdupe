#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/EventArgs.h"
#include "System/EventHandler.h"
#include "System/SmartPointer.h"
#include "System/Exception.h"
#include "System/Collections/Generic/List.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamOptions.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            class ProcessEventArgs : public EventArgs {
            public:
                explicit ProcessEventArgs(const ProcessInfo& proc) : m_proc(proc) {}
                const ProcessInfo& GetProcess() const { return m_proc; }

            private:
                ProcessInfo m_proc;
            };

            class ProcessBatchEventArgs : public EventArgs {
            public:
                explicit ProcessBatchEventArgs(const Collections::Generic::List<ProcessInfo>& lstBatch)
                    : m_lstBatch(lstBatch) {}
                const Collections::Generic::List<ProcessInfo>& GetBatch() const { return m_lstBatch; }

            private:
                Collections::Generic::List<ProcessInfo> m_lstBatch;
            };

            class ProcessStreamErrorEventArgs : public EventArgs {
            public:
                explicit ProcessStreamErrorEventArgs(const String& sErrorMessage)
                    : m_sErrorMessage(sErrorMessage) {}
                String GetErrorMessage() const { return m_sErrorMessage; }

            private:
                String m_sErrorMessage;
            };

            class ProcessStreamer : public virtual Object {
            private:
                class Impl;
                SmartPointer<Impl> m_pImpl;

            public:
                EventHandler<ProcessEventArgs>& ProcessDiscovered;
                EventHandler<ProcessBatchEventArgs>& BatchReady;
                EventHandler<ProcessEventArgs>& ProcessUpdated;
                EventHandler<>& Completed;
                EventHandler<ProcessStreamErrorEventArgs>& Error;

                DOTNETDUPE_API explicit ProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions());
                DOTNETDUPE_API ~ProcessStreamer() override;

                DOTNETDUPE_API void Start();
                DOTNETDUPE_API void Cancel();
                DOTNETDUPE_API bool IsRunning() const;
                DOTNETDUPE_API ProcessStreamOptions GetOptions() const;
            };

        }
    }
}
