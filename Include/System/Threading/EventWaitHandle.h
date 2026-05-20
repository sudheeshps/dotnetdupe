#pragma once
#include "System/Threading/WaitHandle.h"
#include <mutex>
#include <condition_variable>
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class EventWaitHandle : public WaitHandle {
            public:
                DOTNETDUPE_API EventWaitHandle(bool initialState, bool manualReset);
                DOTNETDUPE_API virtual ~EventWaitHandle();
                DOTNETDUPE_API bool Set();
                DOTNETDUPE_API bool Reset();
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
            protected:
                bool _state;
                bool _manualReset;
                std::mutex _mutex;
                std::condition_variable _cv;
            };
        }
    }
}