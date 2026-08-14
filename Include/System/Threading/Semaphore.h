#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"
#include "System/SmartPointer.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class Semaphore : public LockWaitHandle {
            public:
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount);
                DOTNETDUPE_API Semaphore(const String& sName, int initialCount = 0, int maximumCount = 1, bool openAlways = true);
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways = true);
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways, bool& bCreatedNew);
                DOTNETDUPE_API ~Semaphore() override;
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                DOTNETDUPE_API static SmartPointer<Semaphore> OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<Semaphore>& pResult);
                struct Impl;
            private:
                int _count;
                int _maxCount;
                String _name;
                void* _hHandle;
                Impl* _pImpl;
            };
        }
    }
}