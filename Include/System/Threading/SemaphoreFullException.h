/// \file SemaphoreFullException.h
/// \brief Exception thrown when the Semaphore::Release method is called on a full semaphore.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class SemaphoreFullException
            /// \brief Exception thrown when the Semaphore::Release method is called on a semaphore whose count is already at the maximum.
            ///
            /// Mirrors .NET System.Threading.SemaphoreFullException per ECMA-335.
            class SemaphoreFullException : public SystemException {
            public:
                /// \brief Initializes a new instance of the SemaphoreFullException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API SemaphoreFullException(const String& sMessage) : SystemException(sMessage) {}
            };

        }
    }
}
