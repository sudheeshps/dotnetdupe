#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class SemaphoreSlim : public LockWaitHandle {
            public:
                DOTNETDUPE_API SemaphoreSlim(int initialCount);
                DOTNETDUPE_API SemaphoreSlim(int initialCount, int maximumCount);
                DOTNETDUPE_API ~SemaphoreSlim() override;
                
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                DOTNETDUPE_API int Release(int releaseCount = 1) override;
                DOTNETDUPE_API int GetCurrentCount() const;
                struct Impl;
            private:
                int _count;
                int _maxCount;
                Impl* _pImpl;
            };
        }
    }
}