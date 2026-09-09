#pragma once
#include "Common.h"
#include "System/Threading/WaitHandle.h"
#include "System/SmartPointer.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class Mutex
            /// \brief A synchronization primitive that can also be used for interprocess synchronization.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.61 and Win32 / POSIX synchronization
            /// semantics. Supports named cross-process mutexes on Windows and high-performance local timed
            /// mutexes on POSIX systems.
            class Mutex : public LockWaitHandle {
            public:
                /// \brief Initializes a new instance of the Mutex class with default properties.
                DOTNETDUPE_API Mutex();

                /// \brief Initializes a new instance of the Mutex class with a Boolean value that indicates whether the calling thread should have initial ownership.
                /// \param bInitiallyOwned true to give the calling thread initial ownership of the named system mutex; otherwise, false.
                DOTNETDUPE_API Mutex(bool bInitiallyOwned);

                /// \brief Initializes a new instance of the Mutex class with a name.
                /// \param sName The name of the Mutex.
                /// \param bInitiallyOwned true to give the calling thread initial ownership; otherwise, false.
                /// \param openAlways true to open existing or create new; false to fail if existing.
                DOTNETDUPE_API Mutex(const String& sName, bool bInitiallyOwned = false, bool openAlways = true);

                /// \brief Initializes a new instance of the Mutex class with initial ownership and name.
                /// \param bInitiallyOwned true to give initial ownership; otherwise, false.
                /// \param sName The name of the Mutex.
                /// \param openAlways true to open existing or create new; false to fail if existing.
                DOTNETDUPE_API Mutex(bool bInitiallyOwned, const String& sName, bool openAlways = true);

                /// \brief Initializes a new instance of the Mutex class with creation status output.
                /// \param bInitiallyOwned true to give initial ownership; otherwise, false.
                /// \param sName The name of the Mutex.
                /// \param openAlways true to open existing or create new.
                /// \param bCreatedNew When this method returns, contains true if a local mutex was created or a system mutex was created; false if the system mutex already existed.
                DOTNETDUPE_API Mutex(bool bInitiallyOwned, const String& sName, bool openAlways, bool& bCreatedNew);

                /// \brief Destructor releasing encapsulated mutex resources.
                DOTNETDUPE_API ~Mutex() override;

                /// \brief Blocks the current thread until the current WaitHandle receives a signal.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne() override;

                /// \brief Blocks the current thread until the current instance receives a signal, using an integer to measure the time interval.
                /// \param millisecondsTimeout The number of milliseconds to wait, or Timeout::Infinite (-1) to wait indefinitely.
                /// \return true if the current instance receives a signal; otherwise, false.
                DOTNETDUPE_API bool WaitOne(int millisecondsTimeout) override;

                /// \brief Releases the Mutex once.
                /// \param releaseCount The number of times to release the mutex (default 1).
                /// \return 0 upon successful release.
                DOTNETDUPE_API int Release(int releaseCount = 1) override;

                /// \brief Opens an existing named system mutex.
                /// \param sName The name of the system mutex to open.
                /// \return An object that represents the named system mutex.
                /// \throws WaitHandleCannotBeOpenedException If the named mutex does not exist.
                DOTNETDUPE_API static SmartPointer<Mutex> OpenExisting(const String& sName);

                /// \brief Opens an existing named system mutex, and returns a value that indicates whether the operation succeeded.
                /// \param sName The name of the system mutex to open.
                /// \param pResult When this method returns, contains a Mutex object if the call succeeded, or null if it failed.
                /// \return true if the named mutex was opened successfully; otherwise, false.
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