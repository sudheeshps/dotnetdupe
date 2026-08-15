#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Action.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            typedef Action<> ThreadStart;
            typedef Action<Object*> ParameterizedThreadStart;

            class Thread : public Object {
            public:
                DOTNETDUPE_API Thread(ThreadStart start);
                DOTNETDUPE_API Thread(ParameterizedThreadStart start);
                DOTNETDUPE_API ~Thread() override;

                DOTNETDUPE_API void Start();
                DOTNETDUPE_API void Start(Object* parameter);
                DOTNETDUPE_API void Join();
                DOTNETDUPE_API bool Join(int millisecondsTimeout);
                
                DOTNETDUPE_API static void Sleep(int millisecondsTimeout);
                
                DOTNETDUPE_API bool IsAlive() const;
                DOTNETDUPE_API String GetName() const;
                DOTNETDUPE_API void SetName(const String& name);
                
                DOTNETDUPE_API static Thread* GetCurrentThread();
                DOTNETDUPE_API static int GetCurrentThreadId();

            private:
                Thread(); // For internal use (main thread wrapper)
                static SmartPointer<Thread> CreateCurrentThreadWrapper();

                struct Impl;
                Impl* m_pImpl;

                static thread_local Thread* _currentThread;
                
                void ThreadMain(Object* parameter);
            };
        }
    }
}
