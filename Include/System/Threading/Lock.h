#pragma once
#include "Common.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreSlim.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            // Primary template
            template <typename T>
            class Lock {
            public:
                Lock(T& syncObject, int millisecondsTimeout = -1, int releaseCount = -1) 
                    : _syncObject(syncObject), _releaseCount(releaseCount == -1 ? 1 : releaseCount) {
                    
                    if (millisecondsTimeout == -1) {
                        _syncObject.WaitOne();
                    } else {
                        _syncObject.WaitOne(millisecondsTimeout);
                    }
                }
                ~Lock() {
                    _syncObject.Release(_releaseCount);
                }
            private:
                T& _syncObject;
                int _releaseCount;
            };

            // --- Specialization for CriticalSection ---
            template <>
            class Lock<CriticalSection> {
            public:
                Lock(CriticalSection& cs, int millisecondsTimeout = -1, int releaseCount = -1) 
                    : _cs(cs) {
                    _cs.Enter();
                }
                ~Lock() {
                    _cs.Leave();
                }
            private:
                CriticalSection& _cs;
            };

            // Type Aliases for convenience
            typedef Lock<CriticalSection> CriticalSectionLock;
            typedef Lock<Mutex> MutexLock;
            typedef Lock<Semaphore> SemaphoreLock;
            typedef Lock<SemaphoreSlim> SemaphoreSlimLock;
        }
    }
}
