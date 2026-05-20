#pragma once
#include "System/Threading/WaitHandle.h"
#include <mutex>
#include <condition_variable>
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class SemaphoreSlim : public LockWaitHandle {
            public:
                DOTNETDUPE_API SemaphoreSlim(int initialCount);
                DOTNETDUPE_API SemaphoreSlim(int initialCount, int maximumCount);
                DOTNETDUPE_API virtual ~SemaphoreSlim();
                
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                DOTNETDUPE_API int Release(int releaseCount = 1) override;
                DOTNETDUPE_API int GetCurrentCount() const;
            private:
                int _count;
                int _maxCount;
                mutable std::mutex _mutex;
                std::condition_variable _cv;
            };
        }
    }
}