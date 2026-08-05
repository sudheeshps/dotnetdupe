#pragma once
#include "System/Threading/WaitHandle.h"
#include "System/String.h"
#include <mutex>
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
                DOTNETDUPE_API virtual ~Mutex();
                DOTNETDUPE_API bool WaitOne() override;
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                DOTNETDUPE_API static Mutex* OpenExisting(const String& sName);
                DOTNETDUPE_API static bool TryOpenExisting(const String& sName, Mutex*& pResult);
            private:
                String _name;
                void* _hHandle;
                std::timed_mutex _mutex;
            };
        }
    }
}