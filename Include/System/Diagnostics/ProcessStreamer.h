#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Action.h"
#include "System/Exception.h"
#include "System/Collections/Generic/List.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamOptions.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            class IProcessObserver : public virtual Object {
            public:
                DOTNETDUPE_API virtual ~IProcessObserver() = default;
                DOTNETDUPE_API virtual void OnProcessDiscovered(const ProcessInfo& proc) = 0;
                DOTNETDUPE_API virtual void OnBatchReady(const Collections::Generic::List<ProcessInfo>& lstBatch) = 0;
                DOTNETDUPE_API virtual void OnProcessUpdated(const ProcessInfo& proc) = 0;
                DOTNETDUPE_API virtual void OnCompleted() = 0;
                DOTNETDUPE_API virtual void OnError(const Exception& ex) = 0;
            };

            class ProcessStreamer : public virtual Object {
            public:
                DOTNETDUPE_API explicit ProcessStreamer(const ProcessStreamOptions& options = ProcessStreamOptions());
                DOTNETDUPE_API ~ProcessStreamer() override;

                DOTNETDUPE_API void OnProcess(const Action<const ProcessInfo&>& fnOnProcess);
                DOTNETDUPE_API void OnBatch(const Action<const Collections::Generic::List<ProcessInfo>&>& fnOnBatch);
                DOTNETDUPE_API void OnProcessUpdated(const Action<const ProcessInfo&>& fnOnUpdated);
                DOTNETDUPE_API void OnCompleted(const Action<>& fnOnCompleted);
                DOTNETDUPE_API void OnError(const Action<const Exception&>& fnOnError);

                DOTNETDUPE_API void Subscribe(const SmartPointer<IProcessObserver>& pObserver);

                DOTNETDUPE_API void Start();
                DOTNETDUPE_API void Cancel();
                DOTNETDUPE_API bool IsRunning() const;
                DOTNETDUPE_API ProcessStreamOptions GetOptions() const;

            private:
                class Impl;
                SmartPointer<Impl> m_pImpl;
            };

        }
    }
}
