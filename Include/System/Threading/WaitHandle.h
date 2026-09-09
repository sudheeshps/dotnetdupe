/// \file WaitHandle.h
/// \brief Base abstract classes for synchronization wait handles.

#pragma once

#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class WaitHandle
            /// \brief Encapsulates operating system-specific objects that wait for exclusive access to shared resources.
            ///
            /// Serves as the abstract base class for synchronization primitives like Mutex,
            /// AutoResetEvent, ManualResetEvent, and Semaphore per ECMA-335.
            class WaitHandle : public Object {
            public:
                /// \brief Releases resources used by the WaitHandle.
                DOTNETDUPE_API virtual ~WaitHandle() { }

                /// \brief Blocks the current thread until the current WaitHandle receives a signal.
                /// \return True if the current instance receives a signal.
                DOTNETDUPE_API virtual bool WaitOne() = 0;

                /// \brief Blocks the current thread until the current WaitHandle receives a signal, using a 32-bit signed integer for timeout.
                /// \param millisecondsTimeout The number of milliseconds to wait, or -1 to wait indefinitely.
                /// \return True if the current instance receives a signal; otherwise false.
                DOTNETDUPE_API virtual bool WaitOne(int millisecondsTimeout) = 0;
            };

            /// \class LockWaitHandle
            /// \brief Abstract base class for wait handles that support releasing acquire counts.
            class LockWaitHandle : public WaitHandle {
            public:
                /// \brief Releases resources used by the LockWaitHandle.
                DOTNETDUPE_API virtual ~LockWaitHandle() { }

                /// \brief Exits the wait handle and returns the previous count.
                /// \param releaseCount The number of times to exit the wait handle.
                /// \return The previous count of the wait handle.
                DOTNETDUPE_API virtual int Release(int releaseCount = 1) = 0;
            };

        }
    }
}