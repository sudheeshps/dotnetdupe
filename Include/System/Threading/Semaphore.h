#pragma once
#include "System/Threading/WaitHandle.h"
#include <mutex>
#include <condition_variable>
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class Semaphore : public LockWaitHandle {
            public:
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount);
                DOTNETDUPE_API virtual ~Semaphore();
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
                DOTNETDUPE_API int Release(int releaseCount = 1) override;
            private:
                int _count;
                int _maxCount;
                std::mutex _mutex;
                std::condition_variable _cv;
            };
        }
    }
}