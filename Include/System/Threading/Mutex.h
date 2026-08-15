#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"
#include "System/SmartPointer.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class Mutex : public LockWaitHandle {
            public:
                DOTNETDUPE_API Mutex();
                DOTNETDUPE_API Mutex(bool bInitiallyOwned);
                DOTNETDUPE_API Mutex(const String& sName, bool bInitiallyOwned = false, bool openAlways = true);
                DOTNETDUPE_API Mutex(bool bInitiallyOwned, const String& sName, bool openAlways = true);
                DOTNETDUPE_API Mutex(bool bInitiallyOwned, const String& sName, bool openAlways, bool& bCreatedNew);
                DOTNETDUPE_API ~Mutex() override;
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                DOTNETDUPE_API static SmartPointer<Mutex> OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, SmartPointer<Mutex>& pResult);
                struct Impl;
            private:
                String _name;
                void* _hHandle;
                Impl* _pImpl;
            };
        }
    }
}