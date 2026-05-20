#pragma once
#include "System/Object.h"

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
                void* _internalSection; // Pointer to CRITICAL_SECTION to avoid Windows.h in header
            };
        }
    }
}