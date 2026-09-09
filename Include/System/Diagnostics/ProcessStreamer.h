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

            /// \class ProcessEventArgs
            /// \brief Event arguments containing a single process snapshot.
            class ProcessEventArgs : public EventArgs {
            public:
                /// \brief Constructs event args with process info.
                /// \param proc Process information snapshot.
                explicit ProcessEventArgs(const ProcessInfo& proc) : m_proc(proc) {}

                /// \brief Gets the process info.
                /// \return Reference to process information.
                const ProcessInfo& GetProcess() const { return m_proc; }

            private:
                ProcessInfo m_proc;
            };

            /// \class ProcessBatchEventArgs
            /// \brief Event arguments containing a batch of process snapshots.
            class ProcessBatchEventArgs : public EventArgs {
            public:
                /// \brief Constructs batch event args.
                /// \param lstBatch List of process information snapshots.
                explicit ProcessBatchEventArgs(const Collections::Generic::List<ProcessInfo>& lstBatch)
                    : m_lstBatch(lstBatch) {}

                /// \brief Gets the batch list.
                /// \return Reference to list of process info.
                const Collections::Generic::List<ProcessInfo>& GetBatch() const { return m_lstBatch; }

            private:
                Collections::Generic::List<ProcessInfo> m_lstBatch;
            };

            /// \class ProcessStreamErrorEventArgs
            /// \brief Event arguments containing error information encountered during streaming.
            class ProcessStreamErrorEventArgs : public EventArgs {
            public:
                /// \brief Constructs error event args.
                /// \param sErrorMessage Descriptive error message.
                explicit ProcessStreamErrorEventArgs(const String& sErrorMessage)
                    : m_sErrorMessage(sErrorMessage) {}

                /// \brief Gets the error message.
                /// \return Error message string.
                String GetErrorMessage() const { return m_sErrorMessage; }

            private:
                String m_sErrorMessage;
            };

            /// \class ProcessStreamer
            /// \brief Streams system process updates asynchronously with batching and event notifications.
            ///
            /// \details Employs background worker threads and timer loops to poll operating system
            /// process metrics, providing reactive push updates via DotNetDupe::System::EventHandler events.
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

                /// \brief Initializes a new instance of the ProcessStreamer class with options.
                /// \param options Configuration options for streaming intervals and batch sizes.
                DOTNETDUPE_API explicit ProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions());

                /// \brief Destructor stopping background worker thread.
                DOTNETDUPE_API ~ProcessStreamer() override;

                /// \brief Starts background process streaming.
                DOTNETDUPE_API void Start();

                /// \brief Cancels background streaming.
                DOTNETDUPE_API void Cancel();

                /// \brief Gets whether streaming is active.
                /// \return true if running; otherwise, false.
                DOTNETDUPE_API bool IsRunning() const;

                /// \brief Gets the configuration options.
                /// \return Current ProcessStreamOptions.
                DOTNETDUPE_API ProcessStreamOptions GetOptions() const;
            };

        }
    }
}
