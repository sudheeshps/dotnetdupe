/// \file AbandonedMutexException.h
/// \brief Exception thrown when a thread acquires a Mutex abandoned by another thread.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class AbandonedMutexException
            /// \brief Exception thrown when a thread acquires a Mutex object that another thread has abandoned by exiting without releasing it.
            ///
            /// Mirrors .NET System.Threading.AbandonedMutexException per ECMA-335.
            class AbandonedMutexException : public SystemException {
            public:
                /// \brief Initializes a new instance of the AbandonedMutexException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API AbandonedMutexException(const String& sMessage) : SystemException(sMessage) { }
            };

        }
    }
}
