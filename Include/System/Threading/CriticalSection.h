#pragma once
#include "System/Object.h"
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class CriticalSection : public Object {
            public:
                DOTNETDUPE_API CriticalSection();
                DOTNETDUPE_API virtual ~CriticalSection();
                DOTNETDUPE_API void Enter();
                DOTNETDUPE_API void Leave();
                DOTNETDUPE_API bool TryEnter();
            private:
                std::recursive_mutex _mutex;
            };
        }
    }
}