#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"
#include "System/SmartPointer.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class Semaphore
            /// \brief Limits the number of threads that can access a resource or pool of resources concurrently.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.62 specifications.
            /// Supports named system-wide semaphores for interprocess synchronization on Windows,
            /// and mutex/condition variable backed counting semaphores across platforms.
            class Semaphore : public LockWaitHandle {
            public:
                /// \brief Initializes a new instance of the Semaphore class, specifying the initial number of entries and the maximum number of concurrent entries.
                /// \param initialCount The initial number of requests for the semaphore that can be granted concurrently.
                /// \param maximumCount The maximum number of requests for the semaphore that can be granted concurrently.
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount);

                /// \brief Initializes a new instance of the Semaphore class with a name and initial/max count.
                /// \param sName The name of the system semaphore.
                /// \param initialCount The initial number of requests that can be granted.
                /// \param maximumCount The maximum number of requests that can be granted.
                /// \param openAlways true to open existing or create new; false to fail if existing.
                DOTNETDUPE_API Semaphore(const String& sName, int initialCount = 0, int maximumCount = 1, bool openAlways = true);

                /// \brief Initializes a new instance of the Semaphore class with initial/max count and name.
                /// \param initialCount The initial number of requests that can be granted.
                /// \param maximumCount The maximum number of requests that can be granted.
                /// \param sName The name of the system semaphore.
                /// \param openAlways true to open existing or create new; false to fail if existing.
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways = true);

                /// \brief Initializes a new instance of the Semaphore class with creation status output.
                /// \param initialCount The initial number of requests that can be granted.
                /// \param maximumCount The maximum number of requests that can be granted.
                /// \param sName The name of the system semaphore.
                /// \param openAlways true to open existing or create new.
                /// \param bCreatedNew When this method returns, contains true if a local or system semaphore was created; false if already existed.
                DOTNETDUPE_API Semaphore(int initialCount, int maximumCount, const String& sName, bool openAlways, bool& bCreatedNew);

                /// \brief Destructor releasing semaphore resources.
                DOTNETDUPE_API ~Semaphore() override;

                /// \brief Blocks the current thread until the current WaitHandle receives a signal.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne() override;

                /// \brief Blocks the current thread until the current instance receives a signal, using a 32-bit signed integer to specify the time interval.
                /// \param millisecondsTimeout The number of milliseconds to wait, or Timeout::Infinite (-1) to wait indefinitely.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                /// \brief Exits the semaphore and returns the previous count.
                /// \param releaseCount The number of times to exit the semaphore.
                /// \return The count on the semaphore before the Release method was called.
                /// \throws SemaphoreFullException If adding releaseCount would cause the semaphore's count to exceed maximum.
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                /// \brief Opens an existing named system semaphore.
                /// \param sName The name of the system semaphore to open.
                /// \return An object that represents the named system semaphore.
                /// \throws WaitHandleCannotBeOpenedException If the named semaphore does not exist.
                DOTNETDUPE_API static SmartPointer<Semaphore> OpenExisting(const String& sName);

                /// \brief Opens an existing named system semaphore, and returns a value that indicates whether the operation succeeded.
                /// \param sName The name of the system semaphore to open.
                /// \param pResult When this method returns, contains a Semaphore object if the call succeeded, or null if it failed.
                /// \return true if the named semaphore was opened successfully; otherwise, false.
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