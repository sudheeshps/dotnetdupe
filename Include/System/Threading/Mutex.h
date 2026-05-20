#pragma once
#include "System/Threading/WaitHandle.h"
#include <mutex>
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class Mutex : public LockWaitHandle {
            public:
                DOTNETDUPE_API Mutex();
                DOTNETDUPE_API virtual ~Mutex();
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
                DOTNETDUPE_API int Release(int releaseCount = 1) override;
            private:
                std::timed_mutex _mutex;
            };
        }
    }
}