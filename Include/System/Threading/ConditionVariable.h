#pragma once
#include "System/Object.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class CriticalSection;
            
            class ConditionVariable : public Object {
            public:
                DOTNETDUPE_API ConditionVariable();
                DOTNETDUPE_API virtual ~ConditionVariable();
                
                ConditionVariable(const ConditionVariable&) = delete;
                ConditionVariable& operator=(const ConditionVariable&) = delete;
                DOTNETDUPE_API ConditionVariable(ConditionVariable&& other) noexcept;
                DOTNETDUPE_API ConditionVariable& operator=(ConditionVariable&& other) noexcept;
                
                DOTNETDUPE_API void Wait(CriticalSection& cs);
                DOTNETDUPE_API bool Wait(CriticalSection& cs, int millisecondsTimeout);
                
                DOTNETDUPE_API void Pulse();
                DOTNETDUPE_API void PulseAll();
            private:
                struct Impl;
                SmartPointer<Impl> m_pImpl;
            };
        }
    }
}
