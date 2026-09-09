/// \file SynchronizationLockException.h
/// \brief Exception thrown when a method requires synchronization but is called from an unsynchronized block.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class SynchronizationLockException
            /// \brief Exception thrown when a method requires synchronization on a Monitor/lock, but the caller does not own the lock.
            ///
            /// Mirrors .NET System.Threading.SynchronizationLockException per ECMA-335.
            class SynchronizationLockException : public SystemException {
            public:
                /// \brief Initializes a new instance of the SynchronizationLockException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API SynchronizationLockException(const String& sMessage) : SystemException(sMessage) { }
            };

        }
    }
}
