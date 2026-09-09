#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class SemaphoreSlim
            /// \brief Represents a lightweight alternative to Semaphore that limits the number of threads that can access a resource or pool of resources concurrently.
            ///
            /// \details Provides fast, user-mode counting semaphore synchronization for intra-process
            /// concurrency control without kernel transition overhead.
            class SemaphoreSlim : public LockWaitHandle {
            public:
                /// \brief Initializes a new instance of the SemaphoreSlim class, specifying the initial number of requests that can be granted concurrently.
                /// \param initialCount The initial number of requests for the semaphore that can be granted concurrently.
                DOTNETDUPE_API SemaphoreSlim(int initialCount);

                /// \brief Initializes a new instance of the SemaphoreSlim class, specifying the initial and maximum number of requests that can be granted concurrently.
                /// \param initialCount The initial number of requests for the semaphore that can be granted concurrently.
                /// \param maximumCount The maximum number of requests for the semaphore that can be granted concurrently.
                DOTNETDUPE_API SemaphoreSlim(int initialCount, int maximumCount);

                /// \brief Destructor releasing internal synchronization primitives.
                DOTNETDUPE_API ~SemaphoreSlim() override;
                
                /// \brief Blocks the current thread until it can enter the SemaphoreSlim.
                /// \return true upon acquiring entry.
                DOTNETDUPE_API bool WaitOne() override;

                /// \brief Blocks the current thread until it can enter the SemaphoreSlim, using a 32-bit signed integer that specifies the timeout.
                /// \param millisecondsTimeout The number of milliseconds to wait, or Timeout::Infinite (-1) to wait indefinitely.
                /// \return true if the current thread successfully entered the SemaphoreSlim; otherwise, false.
                /// \throws TimeoutException If the wait interval expires before entering.
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                /// \brief Releases the SemaphoreSlim object a specified number of times.
                /// \param releaseCount The number of times to exit the semaphore.
                /// \return The previous count of the SemaphoreSlim.
                /// \throws SemaphoreFullException If the maximum count has been exceeded.
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                /// \brief Gets the number of remaining threads that can enter the SemaphoreSlim object.
                /// \return The number of remaining threads that can enter the semaphore.
                DOTNETDUPE_API int GetCurrentCount() const;

                struct Impl;
            private:
                int _count;
                int _maxCount;
                Impl* _pImpl;
            };
        }
    }
}