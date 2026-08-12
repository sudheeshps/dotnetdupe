#pragma once
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class CriticalSection;
            
            class ConditionVariable : public Object {
            public:
                DOTNETDUPE_API ConditionVariable();
                DOTNETDUPE_API virtual ~ConditionVariable();
                
                DOTNETDUPE_API void Wait(CriticalSection& cs);
                DOTNETDUPE_API bool Wait(CriticalSection& cs, int millisecondsTimeout);
                
                DOTNETDUPE_API void Pulse();
                DOTNETDUPE_API void PulseAll();
            private:
                void* _pData;
            };
        }
    }
}
