#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class CriticalSection : public Object {
            public:
                DOTNETDUPE_API CriticalSection();
                DOTNETDUPE_API ~CriticalSection() override;
                DOTNETDUPE_API void Enter();
                DOTNETDUPE_API void Leave();
                DOTNETDUPE_API bool TryEnter();
            private:
                struct Impl;
                Impl* m_pImpl;
            };
        }
    }
}