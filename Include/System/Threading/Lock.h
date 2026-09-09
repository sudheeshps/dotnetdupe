#pragma once
#include "Common.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreSlim.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class Lock
            /// \brief Provides an RAII-style scoped lock wrapper around synchronization primitives.
            ///
            /// \details Acquires lock ownership on construction and releases ownership automatically
            /// upon destruction (scope exit), preventing deadlocks caused by unhandled exceptions.
            template <typename T>
            class Lock {
            public:
                /// \brief Acquires the synchronization lock on the specified object.
                /// \param syncObject The synchronization object to lock.
                /// \param millisecondsTimeout The timeout in milliseconds (-1 for infinite).
                /// \param releaseCount The number of units to release on exit (-1 defaults to 1).
                Lock(T& syncObject, int millisecondsTimeout = -1, int releaseCount = -1) 
                    : _syncObject(syncObject), _releaseCount(releaseCount == -1 ? 1 : releaseCount) {
                    
                    if (millisecondsTimeout == -1) {
                        _syncObject.WaitOne();
                    } else {
                        _syncObject.WaitOne(millisecondsTimeout);
                    }
                }

                /// \brief Releases the synchronization lock upon destruction.
                ~Lock() {
                    _syncObject.Release(_releaseCount);
                }

            private:
                T& _syncObject;
                int _releaseCount;
            };

            /// \brief Specialization of Lock for CriticalSection objects.
            template <>
            class Lock<CriticalSection> {
            public:
                /// \brief Enters the critical section upon construction.
                /// \param cs The CriticalSection to lock.
                /// \param millisecondsTimeout Not used for CriticalSection.
                /// \param releaseCount Not used for CriticalSection.
                Lock(CriticalSection& cs, int millisecondsTimeout = -1, int releaseCount = -1) 
                    : _cs(cs) {
                    _cs.Enter();
                }

                /// \brief Leaves the critical section upon destruction.
                ~Lock() {
                    _cs.Leave();
                }

            private:
                CriticalSection& _cs;
            };

            /// \typedef CriticalSectionLock
            /// \brief Convenience alias for Lock<CriticalSection>.
            typedef Lock<CriticalSection> CriticalSectionLock;

            /// \typedef MutexLock
            /// \brief Convenience alias for Lock<Mutex>.
            typedef Lock<Mutex> MutexLock;

            /// \typedef SemaphoreLock
            /// \brief Convenience alias for Lock<Semaphore>.
            typedef Lock<Semaphore> SemaphoreLock;

            /// \typedef SemaphoreSlimLock
            /// \brief Convenience alias for Lock<SemaphoreSlim>.
            typedef Lock<SemaphoreSlim> SemaphoreSlimLock;
        }
    }
}
